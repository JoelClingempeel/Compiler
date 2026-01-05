CXX := g++
CXXFLAGS := -std=c++17

TARGET := compile
SRCS := lexer.cc parser.cc code_gen.cc main.cc
OBJS := $(SRCS:.cc=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
