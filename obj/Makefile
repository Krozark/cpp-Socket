OBJ = $(wildcard *.o)
OBJ := $(filter-out $(SERVER).o, $(OBJ))
OBJ := $(filter-out $(CLIENT).o, $(OBJ))
OBJ := $(filter-out Srv-Server.o, $(OBJ))

all: $(EXEC)
	
$(SERVER): $(OBJ) $(SERVER).o Srv-Server.o
	$(CC) $^ $(FLAGS) -o "$(TOP)/$(SERVER)"

$(CLIENT): $(OBJ) $(CLIENT).o
	$(CC) $^ $(FLAGS) -o "$(TOP)/$(CLIENT)"

clean:
	@rm -f *.o
