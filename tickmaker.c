#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

//
//
//
static const char *gProgramName;

//
//
//
static bool isNumber (const char *s)
{
  while (*s)
    if (!isdigit (*s++))
      return false;

  return true;
}

//
//
//
static void usageAndExit (void) __attribute__ ((noreturn));
static void usageAndExit (void)
{
  printf ("\n");
  printf ("usage: %s [-h] [<high time> <low time> <count>]\n", gProgramName);
  printf ("\n");
  printf ("  -h          - This help\n");
  printf ("  <high time> - Time in milliseconds GPIO6 is high (1..999)\n");
  printf ("  <low time>  - Time in milliseconds GPIO6 is low (1..999)\n");
  printf ("  <count>     - Number of high/low cycles to perform (1..%d)\n", INT_MAX);
  printf ("\n");

  exit (1);
}

static void errorAndExit (const char *fmt, ...) __attribute__ ((noreturn));
static void errorAndExit (const char *fmt, ...)
{
  va_list va;

  va_start (va, fmt);
  vfprintf (stderr, fmt, va);
  va_end (va);

  exit (1);
}

//
//
//
int main (int argc, char **argv)
{
  int high;
  int low;
  int count;
  const char *gpio66Direction = "/sys/class/gpio/gpio66/direction";
  const char *gpio66Value = "/sys/class/gpio/gpio66/value";
  FILE *fp;

  (gProgramName = strrchr (argv [0], '/')) ? ++gProgramName : (gProgramName = argv [0]);

  if ((argc == 2) && !strcmp (argv [1], "-h"))
    usageAndExit ();
  if ((argc != 4) || !isNumber (argv [1]) || !isNumber (argv [2]) || !isNumber (argv [3]))
    errorAndExit ("Invalid arguments. Use -h for help\n");

  high  = atoi (argv [1]);
  low   = atoi (argv [2]);
  count = atoi (argv [3]);

  if ((high < 1) || (high > 999) || (low < 1) || (low > 999) || (count < 1) || (count > INT_MAX))
    errorAndExit ("Invalid arguments. Use -h for help\n");

  //
  //
  //
  if (!((fp = fopen (gpio66Direction, "wb"))))
    errorAndExit ("Can't open '%s'\n", gpio66Direction);

  fprintf (fp, "out");
  fclose (fp);

  //
  //
  //
  while (count--)
  {
    if (!((fp = fopen (gpio66Value, "wb"))))
      errorAndExit ("Can't open '%s'\n", gpio66Direction);
    fprintf (fp, "1");
    fclose (fp);

    usleep ((high * 1000) - 600);

    if (!((fp = fopen (gpio66Value, "wb"))))
      errorAndExit ("Can't open '%s'\n", gpio66Direction);
    fprintf (fp, "0");
    fclose (fp);

    usleep ((low * 1000) - 600);
  }

  fclose (fp);

  exit (0);
}
