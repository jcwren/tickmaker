#---------------------------------------------------
# Compiler options
#---------------------------------------------------

# add location to CC_PREFIX if required
CC_PREFIX = 
CC = ${CC_PREFIX}gcc
AR = ${CC_PREFIX}ar
LD = ${CC_PREFIX}ld

all: tickmaker

#---------------------------------------------------
# Compile Application
#---------------------------------------------------

tickmaker : tickmaker.c 
	@${CC} -W -Wall tickmaker.c -o tickmaker

.PHONY: clean
clean :
	rm -rf tickmaker *.o
