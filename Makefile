CC = g++

all: main

main: main.cpp
	$(CC) -w main.cpp -o main
clean:
	rm -rf main