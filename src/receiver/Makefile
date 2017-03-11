CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -ggdb
OBJFILES=	paudio.o\
		resender.o\
		main.o
BINARY=IBP
DEPLIBS=threadSafety.h\
		paudio.h\
		typedefs.h\
		resender.h
		

${BINARY}:${OBJFILES}
	${CC} ${OBJFILES} -lpthread -lportaudio -o ${BINARY}

paudio.o:paudio.cpp ${DEPLIBS}
	${CC} ${CFLAGS} -c paudio.cpp
	
resender.o:resender.cpp ${DEPLIBS}
	${CC} ${CFLAGS} -c resender.cpp
	
main.o:main.cpp
	${CC} ${CFLAGS} -c main.cpp
	

	
clean:
	rm -rf *.o
	
purge:clean
	rm -rf ${BINARY}
