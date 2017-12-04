#---------------------------------------------------
# Paths etc.
#---------------------------------------------------
LIB_PATH = ./BBBio_lib
INCLUDE_PATH = ./BBBio_lib
LIBRARY = BBBio

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
# Compile Library
#---------------------------------------------------
${LIB_PATH}/libBBBio.a : ${LIB_PATH}/BBBiolib.c ${LIB_PATH}/BBBiolib.h BBBiolib_PWMSS.o BBBiolib_McSPI.o BBBiolib_ADCTSC.o
	${CC} -c ${LIB_PATH}/BBBiolib.c -o ${LIB_PATH}/BBBiolib.o
	${AR} -rs ${LIB_PATH}/libBBBio.a ${LIB_PATH}/BBBiolib.o ${LIB_PATH}/BBBiolib_PWMSS.o ${LIB_PATH}/BBBiolib_McSPI.o ${LIB_PATH}/BBBiolib_ADCTSC.o

BBBiolib_PWMSS.o : ${LIB_PATH}/BBBiolib_PWMSS.c ${LIB_PATH}/BBBiolib_PWMSS.h
	${CC} -c ${LIB_PATH}/BBBiolib_PWMSS.c -o ${LIB_PATH}/BBBiolib_PWMSS.o -W 

BBBiolib_McSPI.o : ${LIB_PATH}/BBBiolib_McSPI.c ${LIB_PATH}/BBBiolib_PWMSS.h
	${CC} -c ${LIB_PATH}/BBBiolib_McSPI.c -o ${LIB_PATH}/BBBiolib_McSPI.o -W

BBBiolib_ADCTSC.o : ${LIB_PATH}/BBBiolib_ADCTSC.c ${LIB_PATH}/BBBiolib_ADCTSC.h
	${CC} -c ${LIB_PATH}/BBBiolib_ADCTSC.c -o ${LIB_PATH}/BBBiolib_ADCTSC.o -W


#---------------------------------------------------
# Compile Application
#---------------------------------------------------

tickmaker : tickmaker.c ${LIB_PATH}/libBBBio.a
	${CC} -o tickmaker tickmaker.c -L ${LIB_PATH} -l${LIBRARY}

.PHONY: clean
clean :
	rm -rf tickmaker *.o ${LIB_PATH}/*.o ${LIB_PATH}/libBBBio.a  
