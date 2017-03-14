CC=g++
CFLAGS= -std=c++11 -Wall -pedantic -ggdb
SOURCES=$(shell find -name "*.cpp" | sed 's/\.\///')
OBJFILES=$(SOURCES:.cpp=.o)
BINARY=sender
HEADERS=threadSafety.h typedefs.h
DEPS=$(OBJFILES:.o=.dep)

all:$(BINARY)

ifneq ($(MAKECMDGOALS), clean)
    -include $(DEPS)
endif

$(BINARY):$(OBJFILES)
	@echo "Linking $(BINARY)"; \
	${CC} ${OBJFILES} -lpthread -lportaudio -o ${BINARY}

%.dep:$(SOURCES)
	@$(CC) $(CFLAGS) -MM $*.cpp| sed -e 's@^\(.*\)\.o:@\1.dep \1.o:@' > $@ 
	
	
%.o:%.cpp
	@echo "Compiling $@"; \
	$(CC) -c  $(CFLAGS) $< -o $@ 
	
clean:
	rm -rf *.o *.dep
	
purge:clean
	rm -rf ${BINARY}

