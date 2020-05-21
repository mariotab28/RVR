CC=g++
CFLAGS=-g -I. -std=c++11
#DEPS = Socket.h Chat.h
#OBJ = Server.o Chat.o
LIBS=-lpthread -lsfml-graphics -lsfml-window -lsfml-system

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: BTServer

BTServer: Server.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

#cc: $(OBJ) ChatClient.o
	#g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o BTServer

