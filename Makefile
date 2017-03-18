CFLAGS= -Wall -Wextra -Werror -g -std=c++11
LFLAGS= -lsfml-graphics -lsfml-window -lsfml-system

all: main.out

run: all
	./main.out

clean: 
	@rm *.o *.exe *.out

main.out: main.cpp tilemap.o game-object.o render.o
	g++ $(CFLAGS) main.cpp render.o game-object.o tilemap.o $(LFLAGS) -o main.out

game-object.o: game-object.cpp game-object.hpp tilemap.o
	g++ $(CFLAGS) -c game-object.cpp -o game-object.o

render.o: render.hpp render.cpp game-object.o tilemap.o
	g++ $(CFLAGS) -c render.cpp -o render.o	

tilemap.o: tilemap.cpp tilemap.hpp
	g++ $(CFLAGS) -c tilemap.cpp -o tilemap.o
