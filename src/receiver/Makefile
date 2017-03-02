CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -ggdb
OBJFILES=	resender.o\
		    main.o
BINARY=IBP

${BINARY}:${OBJFILES}
	${CC} ${OBJFILES} -lpthread -lportaudio -o ${BINARY}

resender.o:resender.cpp threadSafety.h
	${CC} ${CFLAGS} -c resender.cpp
	
main.o:main.cpp
	${CC} ${CFLAGS} -c main.cpp
	

	
clean:
	rm -rf *.o
	
purge:clean
	rm -rf ${BINARY}
