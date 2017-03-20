CC=g++
CPP_FILES := $(wildcard src/*.cpp)
H_FILES := $(wildcard src/*.h)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))
DEP_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.dep)))

CFLAGS+= -O3 -g -Wall -std=c++0x
LDFLAGS = -std=c++0x -g

.PHONY: all clean main test

all: bin/main

obj/%.o:src/%.cpp
	g++ ${CFLAGS} -o $@ -c $<

bin/main: ${OBJ_FILES}
	g++ $^ -o $@ ${LDFLAGS}

clean:
	@rm -f obj/* bin/* src/*~ *~ core

include ${DEP_FILES}

obj/%.dep:src/%.cpp
	@set -e; rm -f $@; \
	gcc -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

test: bin/main
	@./bin/main ./data/karate.graph
