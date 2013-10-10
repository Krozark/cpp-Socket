OBJ = $(wildcard *.o)
OBJ := $(filter-out $(SERVER).o, $(OBJ))
OBJ := $(filter-out $(CLIENT).o, $(OBJ))

all: $(SERVER) $(CLIENT)
	
$(SERVER): $(OBJ) $(SERVER).o
	$(CC) $^ $(FLAGS) -o "$(TOP)/$(SERVER)"

$(CLIENT): $(OBJ) $(CLIENT).o
	$(CC) $^ $(FLAGS) -o "$(TOP)/$(CLIENT)"

clean:
	@rm -f *.o
