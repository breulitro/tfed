all:
	g++ base.cpp -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/SDL  -lSDL -lpthread   -g3
