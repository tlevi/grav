.PHONY: clean
.SUFFIXES:
.SUFFIXES: .d .cpp .o

CFLAGS := -pipe -Wall -Werror -O3 -march=native -fopenmp -s -fno-rtti \
-fno-inline-functions -finline-small-functions -ffast-math -funsafe-math-optimizations -fno-trapping-math -mrecip

LFLAGS := 
SRC := $(shell find src/ -iname *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))

all: glut
debug: glut

glut : CFLAGS +=-DAPI_GLUT -I/usr/include/GL
glut : LFLAGS +=-lglut
sdl : CFLAGS +=-DAPI_SDL $(shell sdl-config --cflags)
sdl : LFLAGS +=$(shell sdl-config --libs)
debug: CFLAGS :=$(patsubst -s, -g, $(CFLAGS))


%.o: %.cpp
	g++ $(CFLAGS) -c $*.cpp -o $*.o
	@g++ -MM $(CFLAGS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

glut: $(OBJ) grav

sdl: $(OBJ) grav

grav: $(OBJ)
	g++ $(LFLAGS) $(OBJ) -o ./grav

clean:
	@rm -f `find src/ -iname *.o` `find src/ -iname *.d` ./grav

ifneq "$(MAKECMDGOALS)" "clean"
-include $(SRC:.cpp=.d)
endif
