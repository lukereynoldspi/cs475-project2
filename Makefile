CC=g++
CFLAGS=-lm -fopenmp

all: proj02

proj01: proj02.cpp
	$(CC) $(CFLAGS) proj02.cpp -o proj02

clean:
	rm -f proj02
