CC=g++
CFLAGS=-lm -fopenmp

all: proj01

proj01: proj01.cpp
	$(CC) $(CFLAGS) proj01.cpp -o proj01

clean:
	rm -f proj01
