CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -ggdb
OBJFILES=	safeQ.o
			resender.o\
		    main.o
BINARY=IBP

${BINARY}:${OBJFILES}
	${CC} ${OBJFILES} -lpthread -o ${BINARY}

safeQ.o:safeQ.cpp
	${CC} ${CFLAGS} -c safeQ.cpp

resender.o:resender.cpp
	${CC} ${CFLAGS} -c resender.cpp
	
main.o:main.cpp
	${CC} ${CFLAGS} -c main.cpp
	

	
clean:
	rm -rf *.o
	
purge:clean
	rm -rf ${BINARY}
