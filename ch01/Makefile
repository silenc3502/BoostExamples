PWD = $(shell pwd)

LIBS = -lboost_program_options

SRCS = $(wildcard *.cpp)
TARGET = $(SRCS:%.cpp=%)

all: $(TARGET)
	@echo $(TARGET)

$(TARGET): %: %.cpp
	$(CXX) -o $@ $< $(LIBS)

clean:
	rm -rf $(TARGET)
