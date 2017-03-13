CC=g++
CFLAGS= -MMD -std=c++11 -Wall -pedantic -ggdb
SOURCES=$(shell find -name "*.cpp" | sed 's/\.\///')
OBJFILES=$(SOURCES:.cpp=.o)
BINARY=IBP
HEADERS=threadSafety.h typedefs.h
	

all:$(BINARY)

$(BINARY):$(OBJFILES)
	@echo "Linking $(BINARY)"; \
	${CC} ${OBJFILES} -lpthread -lportaudio -o ${BINARY}

%.o:%.cpp ${HEADERS}
	@echo "Compiling $@"; \
	${CC} -c  ${CFLAGS} $< -o $@ 
	
clean:
	rm -rf *.o
	
purge:clean
	rm -rf ${BINARY}

