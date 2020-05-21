CC=g++
CFLAGS=-g -I. -std=c++11
DEPS = GameWorld.h GameObject.h 
OBJ = GameWorld.o GameObject.o
LIBS=-lpthread -lsfml-graphics -lsfml-window -lsfml-system

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: BTServer

BTServer: $(OBJ) Server.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

#BTClient: $(OBJ) Client.o
	#g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o BTServer 
#BTClient

