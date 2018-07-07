CC=g++

SOURCE=./src/main.cpp

EXEC=./bin/run

FLAGS=-std=c++11 -Wall

all: entity renderer world
	$(CC) -I./include $(SOURCE) ./bin/*.o -o $(EXEC) $(FLAGS) -lX11

world:
	$(CC) -I./include -c ./src/world.cpp -o ./bin/world.o $(FLAGS)

entity:
	$(CC) -I./include -c ./src/entity.cpp -o ./bin/entity.o $(FLAGS)

renderer:
	$(CC) -I./include -c ./src/renderer.cpp -o ./bin/renderer.o $(FLAGS)

clean:
	rm ./bin/*
