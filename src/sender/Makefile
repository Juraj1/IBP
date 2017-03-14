CC=g++
INCLUDE=-I../include/
CFLAGS= -std=c++11 -Wall -pedantic -ggdb $(INCLUDE)
SOURCES=$(shell find -name "*.cpp" | sed 's/\.\///')
OBJFILES=$(SOURCES:.cpp=.o)
BINARY=sender
DEPS=$(OBJFILES:.o=.dep)

all:$(BINARY)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), purge)
-include $(DEPS)
endif
endif

$(BINARY):$(OBJFILES)
	@echo "Linking $(BINARY)"; \
	${CC} ${OBJFILES} -lpthread -lportaudio -o ${BINARY}

%.dep:%.cpp
	@echo "Generating dependencies for $<"; \
	$(CC) $(CFLAGS) -MM $< -MF $*.dep
	
	
%.o:%.cpp
	@echo "Compiling $@"; \
	$(CC) -c  $(CFLAGS) $< -o $@ 
	
clean:
	rm -rf *.o *.dep
	
purge:clean
	rm -rf ${BINARY}

