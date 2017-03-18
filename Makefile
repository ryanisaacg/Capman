CFLAGS= -Wall -Wextra -Werror -g -std=c++11
LFLAGS= -lsfml-graphics -lsfml-window -lsfml-system

all: main.out

run: all
	./main.out

clean: 
	@rm *.o *.exe *.out

main.out: main.cpp tilemap.o game-object.o 
	g++ $(CFLAGS) main.cpp game-object.o tilemap.o $(LFLAGS) -o main.out

game-object.o: game-object.cpp game-object.hpp tilemap.o
	g++ $(CFLAGS) -c game-object.cpp -o game-object.o

tilemap.o: tilemap.cpp tilemap.hpp
	g++ $(CFLAGS) -c tilemap.cpp -o tilemap.o
