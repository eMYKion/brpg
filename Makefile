CC=g++

SOURCE=./src/main.cpp

EXEC=./bin/run

FLAGS=-std=c++11 -Wall

all: entity renderer
	$(CC) -I./include $(SOURCE) ./bin/entity.o ./bin/renderer.o -o $(EXEC) $(FLAGS) -lX11

entity:
	$(CC) -I./include -c ./src/entity.cpp -o ./bin/entity.o $(FLAGS)

renderer:
	$(CC) -I./include -c ./src/renderer.cpp -o ./bin/renderer.o $(FLAGS)

clean:
	rm ./bin/*