SRVOBJ = $(wildcard srv-*.o)
CLIOBJ = $(wildcard cli-*.o)
OBJ = $(wildcard *.o)
OBJ := $(filter-out $(SRVOBJ), $(OBJ))
OBJ := $(filter-out $(CLIOBJ), $(OBJ))

all: $(EXEC)
	
$(SERVER): $(OBJ) $(SRVOBJ)
	$(CC) $^ $(FLAGS) -o "$(TOP)/$(SERVER)"

$(CLIENT): $(OBJ) $(CLIOBJ)
	$(CC) $^ $(FLAGS) -o "$(TOP)/$(CLIENT)"

clean:
	@rm -f *.o
