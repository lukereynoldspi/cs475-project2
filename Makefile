CC = g++
CFLAGS = -Wall -std=c++11 -fopenmp
LDFLAGS = -fopenmp

all: proj02

proj02: proj02.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) proj02.cpp -o proj02

clean:
	rm -f proj02
