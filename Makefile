CFLAGS:= -Wall -Wextra -Werror -g -std=c++11
LFLAGS:= -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system 

all: main.out

run: all
	LD_LIBRARY_PATH=. ./main.out

clean: 
	@rm *.o *.exe *.out

main.out: main.cpp tilemap.o object.o level.o music.o game.o
	g++ $(CFLAGS) main.cpp object.o tilemap.o level.o music.o game.o $(LFLAGS) -o main.out

object.o: object.cpp object.hpp tilemap.o
	g++ $(CFLAGS) -c object.cpp -o object.o

music.o: music.hpp music.cpp
	g++ $(CFLAGS) -c music.cpp -o music.o

tilemap.o: tilemap.cpp tilemap.hpp
	g++ $(CFLAGS) -c tilemap.cpp -o tilemap.o

level.o: level.hpp level.cpp tilemap.o object.o
	g++ $(CFLAGS) -c level.cpp -o level.o

game.o: game.hpp game.cpp screen.hpp object.o music.o render.o tilemap.o level.o
	g++ $(CFLAGS) -c game.cpp -o game.o
