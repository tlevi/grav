all:
	@g++ -I/usr/include/GL -O3 -s -march=native -Wall -Werror -fopenmp -lglut *.cpp -o ./grav

clean:
	@rm -f ./grav
