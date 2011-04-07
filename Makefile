SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)

CXX = g++
CXXFLAGS ?= -Wall -Werror -O3 -march=native -s -fopenmp

LDFLAGS ?= 
SED = sed
MV = mv
RM = rm

.SUFFIXES: .o .cpp

app = grav

src = main.cpp \
	physic.cpp \
	object.cpp \
	render.cpp \
#	quadtree.cpp \
#	phystree.cpp \
#	cdetect.cpp \

objects = $(patsubst %.cpp, %.o, $(src))
dependencies = $(subst .o,.d,$(objects))

define make-depend
  $(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -M $1 | \
  $(SED) -e 's,\($(notdir $2)\) *:,$(dir $2)\1: ,' > $3.tmp
  $(SED) -e 's/#.*//' \
      -e 's/^[^:]*: *//' \
      -e 's/ *\\$$//' \
      -e '/^$$/ d' \
      -e 's/$$/ :/' $3.tmp >> $3.tmp
  $(MV) $3.tmp $3
endef


all: $(app)


$(app): $(objects) 
	@echo Creating $(app) application...
	@$(CXX) $(objects) -o $@ $(CXXFLAGS) $(SDL_LDFLAGS)
	@strip -s $@
    
clean:
	-@$(RM) -f $(objects) $(dependencies) $(app) 2> /dev/null

%.o : %.cpp
	@echo $<
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -c $< -o $(patsubst %.cpp, %.o, $<)


ifneq "$(MAKECMDGOALS)" "clean"
-include $(dependencies)
-include $(test_dependencies)
endif
