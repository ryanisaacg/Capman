CFLAGS:= -Wall -Wextra -Werror -g -std=c++11
LFLAGS:= -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system 

all: main.out

run: all
	LD_LIBRARY_PATH=. ./main.out

clean: 
	@rm *.o *.exe *.out

main.out: main.cpp tilemap.o object.o render.o level.o music.o
	g++ $(CFLAGS) main.cpp render.o object.o tilemap.o level.o music.o $(LFLAGS) -o main.out

object.o: object.cpp object.hpp tilemap.o
	g++ $(CFLAGS) -c object.cpp -o object.o

music.o: music.hpp music.cpp
	g++ $(CFLAGS) -c music.cpp -o music.o

render.o: render.hpp render.cpp object.o tilemap.o
	g++ $(CFLAGS) -c render.cpp -o render.o	

tilemap.o: tilemap.cpp tilemap.hpp
	g++ $(CFLAGS) -c tilemap.cpp -o tilemap.o

level.o: level.hpp level.cpp tilemap.o object.o
	g++ $(CFLAGS) -c level.cpp -o level.o
