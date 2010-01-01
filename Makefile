all:
	@g++ -I/usr/include/GL -O3 -s -march=native -fopenmp -lglut *.cpp -o ./grav

clean:
	@rm -f ./grav
