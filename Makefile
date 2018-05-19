CC=g++

SOURCE=./src/main.cpp

EXEC=./bin/run

FLAGS=-std=c++11 -lX11 -Wall

all: entity
	$(CC) -I./include $(SOURCE) ./bin/entity.o -o $(EXEC) $(FLAGS)

entity:
	$(CC) -I./include -c ./src/entity.cpp -o ./bin/entity.o -std=c++11 -Wall

clean:
	rm ./bin/*