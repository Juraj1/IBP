BINARY=sender
OBJFILES=main.o
CC=g++
CFLAGS=-std=c++11 -ggdb -Wall -pedantic -Wextra

${BINARY}:${OBJFILES}
	${CC} ${OBJFILES} -o ${BINARY}

main.o:main.cpp
	${CC} ${CFLAGS} -c main.cpp

clean:
	rm -rf *.o

purge:clean
	rm -rf ${BINARY}
