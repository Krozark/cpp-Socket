OBJ = $(wildcard *.o)

all: $(SERVER) $(CLIENT)
	
$(SERVER): $(OBJ) serveur.o
	$(CC) $^ $(FLAGS) -o "$(TOP)/$(EXEC)"

$(CLIENT): $(OBJ) client.o
	$(CC) $^ $(FLAGS) -o "$(TOP)/$(EXEC)"

clean:
	@rm -f *.o
