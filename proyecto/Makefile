CC=g++
CFLAGS=-g -I. -std=c++11
DEPS = Socket.h BT.h GameWorld.h GameObject.h Player.h Bullet.h Gear.h
OBJ = Socket.o BT.o GameWorld.o GameObject.o Player.o Bullet.o Gear.o
LIBS=-lpthread -lsfml-graphics -lsfml-window -lsfml-system -lX11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: BTServer BTClient

BTServer: $(OBJ) BTServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

BTClient: $(OBJ) BTClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o BTServer BTClient