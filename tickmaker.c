//
//  BBBiolib came from https://github.com/adwaitnd/BBBIOlib
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <stdarg.h>
#include "BBBio_lib/BBBiolib.h"

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
  printf ("Program must be run as root to achieve accurate timing\n");
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
  struct sched_param param;

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
  if (geteuid ())
    errorAndExit ("Must be root to run this program. Use -h for help\n");

  memset (&param, 0, sizeof (param));
  param.sched_priority = sched_get_priority_max (SCHED_RR);

  if (sched_setscheduler (0, SCHED_RR, &param))
    errorAndExit ("sched_setscheduler() failed, error %d/%s\n", errno, strerror (errno));

  //
  //
  //
  iolib_init ();

  BBBIO_sys_Enable_GPIO (BBBIO_GPIO2);
  BBBIO_GPIO_set_dir (BBBIO_GPIO2, 0, BBBIO_GPIO_PIN_6);
  BBBIO_GPIO_low (BBBIO_GPIO2, BBBIO_GPIO_PIN_6);

  while (count--)
  {
    BBBIO_GPIO_high (BBBIO_GPIO2, BBBIO_GPIO_PIN_6);
    iolib_delay_ms (high);
    BBBIO_GPIO_low (BBBIO_GPIO2, BBBIO_GPIO_PIN_6);
    iolib_delay_ms (low);
  }

  iolib_free ();

  exit (0);
}
