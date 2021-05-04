PWD = $(shell pwd)

LIBS = -lboost_program_options
THREAD_LIBS = -lpthread -lboost_thread -lboost_chrono -lboost_system

BASIC = boost_scoped_ptr.cpp boost_scoped_array.cpp boost_function_fobject.cpp boost_function_fpointer.cpp boost_function_lambda.cpp boost_ptr_container.cpp boost_scope_exit.cpp boost_base_from_member.cpp
THREAD_MODULE = boost_shared_ptr.cpp boost_shared_array.cpp

SRCS = $(wildcard $(BASIC))
BASIC_TARGET = $(SRCS:%.cpp=%)

THREAD_SRCS = $(wildcard $(THREAD_MODULE))
THREAD_TARGET = $(THREAD_SRCS:%.cpp=%)

all: $(BASIC_TARGET) $(THREAD_TARGET)
	@echo $(BASIC_TARGET) $(THREAD_TARGET)

$(BASIC_TARGET): %: %.cpp
	$(CXX) -o $@ $< $(LIBS)

$(THREAD_TARGET): %: %.cpp
	$(CXX) -o $@ $< $(THREAD_LIBS)

clean:
	rm -rf $(BASIC_TARGET) $(THREAD_TARGET)
