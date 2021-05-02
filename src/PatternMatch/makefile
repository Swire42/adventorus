CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
SDLFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer
LFLAGS = -Wall $(DEBUG)
COMP = $(CC) $(CFLAGS)

HEADDIR = /home/victor/Programmation/C++/Headers/
EASYSDLDIR = $(HEADDIR)EasySDL/v6/
COORDSDIR = $(HEADDIR)Geometry/v2/
MYMATHDIR = $(HEADDIR)MyMath/v2/
MY2DFUNCS = $(HEADDIR)My2DFuncs/v1/
GEOMETRYDIR = $(HEADDIR)Geometry/v1/

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
HEAD = $(wildcard *.hpp)

HEADOBJ = $(wildcard $(EASYSDLDIR)*.o) $(wildcard $(COORDSDIR)*.o) $(wildcard $(MYMATHDIR)*.o) $(wildcard $(MY2DFUNCS)*.o) $(wildcard $(GEOMETRYDIR)*.o)

exec : $(OBJ)
	$(CC) $(LFLAGS) $^ $(HEADOBJ) -o $@ $(SDLFLAGS)

main.o : main.cpp $(HEAD)
	$(COMP) -I$(HEADDIR) $< -o $@

%.o : %.cpp %.hpp
	$(COMP) -I$(HEADDIR) $< -o $@

clean :
	rm *.o
	rm exec
