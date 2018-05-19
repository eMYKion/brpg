CC=g++

SOURCE=./src/main.cpp

EXEC=./bin/run

FLAGS=-std=c++11 -lX11 -Wall

all:
	$(CC) $(SOURCE) -o $(EXEC) $(FLAGS)
	
clean:
	rm ./bin/*