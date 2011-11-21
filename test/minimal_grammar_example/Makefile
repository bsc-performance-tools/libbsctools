CPP=g++
CPP_FLAGS=-g -I/usr/local/boost_1_48_0/

all: minimal_test


minimal_test: minimal_test.cpp
	$(CPP) $(CPP_FLAGS) -o $@ $<

clean:
	rm -f minimal_test
