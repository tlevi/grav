CFLAGS := -O3 -march=native -Wall -Werror -s -fopenmp
SRC ?= $(shell find src/ -iname *.cpp)

all: glut

glut:
	@g++ ${CFLAGS} -DAPI_GLUT -I/usr/include/GL -lglut ${SRC} -o ./grav

sdl:
	@g++ ${CFLAGS} -DAPI_SDL `sdl-config --cflags --libs` ${SRC} -o ./grav

clean:
	@rm -f ./grav
