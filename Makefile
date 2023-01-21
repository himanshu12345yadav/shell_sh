CC=g++

all: build

build: main.cpp utils.cpp
	$(CC) main.cpp utils.cpp -o shell.out

clean:
	rm shell.out