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


client.exe: Socket/client-main.cpp Socket/Socket.o Socket/Serializer.o Socket/SocketSerialized.o Socket/SelectManager.o
	$(CC) $^ $(FLAGS) -o $@

serveur.exe: Socket/serveur-main.cpp Socket/Socket.o Socket/Serializer.o Socket/SocketSerialized.o Socket/SelectManager.o
	$(CC) $^ $(FLAGS) -o $@

Socket/Serializer.o : Socket/Serializer.cpp Socket/Serializer.hpp
	$(CC) $(FLAGS) -o "$@" -c "$<"

Socket/Socket.o: Socket/Socket.cpp Socket/Socket.hpp
	$(CC) $(FLAGS) -o "$@" -c "$<"

clean:
	@rm -f *.o
	@rm -f Socket/*.o
	@rm -f *.exe
