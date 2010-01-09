all:
	@g++ -I/usr/include/GL -O3 -s -march=native -Wall -Werror -fopenmp -DAPI_GLUT -lglut `find src/ -iname *.cpp` -o ./grav

glut: all

sdl:
	@g++ `sdl-config --cflags --libs` -O3 -s -march=native -Wall -Werror -fopenmp -DAPI_SDL `find src/ -iname *.cpp` -o ./grav

clean:
	@rm -f ./grav

