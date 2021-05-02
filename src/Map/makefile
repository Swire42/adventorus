CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
COMP = $(CC) $(CFLAGS)

HEADDIR = /home/victor/Programmation/C++/Headers/
EASYSDLDIR = $(HEADDIR)EasySDL/v6/
COORDSDIR = $(HEADDIR)Coords/v2/
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o) $(wildcard $(EASYSDLDIR)*.o) $(wildcard $(COORDSDIR)*.o)

exec : OBJ
	$(CC) $(LFLAGS) $^ -o $@

display.o : map.hpp

%.o : %.cpp %.hpp
	$(COMP) $< -o $@

clean :
	rm *.o
