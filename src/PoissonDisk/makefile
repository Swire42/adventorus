CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
SDLFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf
LFLAGS = -Wall $(DEBUG)
COMP = $(CC) $(CFLAGS)

HEADDIR = /home/victor/Programmation/C++/Headers/
EASYSDLDIR = $(HEADDIR)EasySDL/v6/
COORDSDIR = $(HEADDIR)Coords/v2/
MYMATHDIR = $(HEADDIR)MyMath/v2/

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
HEAD = $(wildcard *.hpp)

HEADOBJ = $(wildcard $(EASYSDLDIR)*.o) $(wildcard $(COORDSDIR)*.o) $(wildcard $(MYMATHDIR)*.o)

display : display.o
	$(CC) $(LFLAGS) $^ $(HEADOBJ) -o $@ $(SDLFLAGS)

display.o : display.cpp $(HEAD)
	g++ -c -I/home/victor/Programmation/C++/Headers $< -o $@

%.o : %.cpp %.hpp
	$(COMP) -I $(HEADDIR) $< -o $@

clean :
	rm *.o
	rm exec
