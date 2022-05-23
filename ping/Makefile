CC = g++
CXXFLAGS = -std=c++11 -O3 -g -Wall

all: ping

ping: ping.cpp
	$(CC) -c $(INC) $(CXXFLAGS) -o $@  $<

clean:
	rm -rf *.o ping

.PHONY: clean all