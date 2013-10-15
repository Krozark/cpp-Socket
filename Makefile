export SERVER = server
export CLIENT = client
export EXEC = $(SERVER)

export CC = g++
INCPATH = -I$(CURDIR)
LIBS = -lpthread 
DEFINES = 
export TOP = $(CURDIR)
export OBJ_DIR = $(TOP)/obj

SRC = $(wildcard *.c*)
OBJ = $(SRC:.c=.o) $(SRC:*.cpp=.o)

SUBDIRS = Socket obj

ifeq ($(EXEC),$(SERVER))
export DEF = $(DEFINES) -DNTW_MODE=2
else
export DEF = $(DEFINES) -DNTW_MODE=1
endif
export FLAGS = -g -std=c++0x $(INCPATH) $(LIBS) $(DEF)

CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(CLEANDIRS)


all: $(OBJ) subdirs

subdirs: $(SUBDIRS)
     
$(SUBDIRS):
	$(MAKE) -C $@


.cpp.o:
	$(CC) $(FLAGS) -o $@ -c $^;
	@mv $@ "$(OBJ_DIR)"

.c.o :
	$(CC) $(FLAGS) -o $@ -c $^;
	@mv $@ "$(OBJ_DIR)"


clean: $(CLEANDIRS)

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
	@rm -f *.o

