CC = g++
INCPATH = 
LIBS = -lpthread 
DEFINES = 
FLAGS = -g -std=c++0x -o3 $(INCPATH) $(LIBS) $(DEFINES)

all: serveur.exe client.exe

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"


client.exe: client-main.cpp Socket.o Serializer.o SocketSerialized.o SelectManager.o
	$(CC) $^ $(FLAGS) -o $@

serveur.exe: serveur-main.cpp Socket.o Serializer.o SocketSerialized.o SelectManager.o
	$(CC) $^ $(FLAGS) -o $@

Serializer.o : Serializer.cpp Serializer.hpp
	$(CC) $(FLAGS) -o "$@" -c "$<"

Socket.o: Socket.cpp Socket.hpp
	$(CC) $(FLAGS) -o "$@" -c "$<"

clean:
	@rm -f *.o
	@rm -f *.exe
