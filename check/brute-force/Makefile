.PHONY: all clean

CXX = g++
CXX_FLAGS = --std=c++0x -g
SRC_FILES = $(wildcard *.cpp)
OBJ_FILES = $(patsubst %.cpp, %.o, $(SRC_FILES))

all: main

main: $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $^ -o $@

%.o : %.cpp $(LIB_FILES)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	@rm -rf *.o main *~ core

test: main
	@./main
