# specify the compiler
CXX = g++

# specify compiler flags
CXXFLAGS = -O3 -std=c++2a -fcoroutines -ffast-math

# specify the target file
TARGET = main

# specify the source files
SRCS = Algo.cpp RandomSearch.cpp RandomWalk.cpp GreedyCycle.cpp LocalSearch.cpp main.cpp 

# specify the object files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<  -o $@

clean:
	$(RM) $(OBJS) $(TARGET)