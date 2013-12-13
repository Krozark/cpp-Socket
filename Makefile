export LIB = cppsocket
export STATIC=$(LIB).a
export SHARED=$(LIB).so

export CC = g++
INCPATH = -I$(CURDIR)/include
LIBS = -lpthread 
DEFINES = -Wall 
export TOP = $(CURDIR)
export OBJ_DIR = $(TOP)/obj

SRC = $(wildcard *.c*)
OBJ = $(SRC:.c=.o) $(SRC:*.cpp=.o)

SUBDIRS = src obj

export FLAGS = -g -std=c++0x $(INCPATH) $(LIBS) $(DEFINES)

CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(CLEANDIRS)


#all: $(OBJ) subdirs

static : src
	$(MAKE) static -C obj

shared :
	$(MAKE) -C src FLAGS="$(FLAGS) -fPIC"
	$(MAKE) shared -C obj

install :
	cp -f $(STATIC) /usr/local/lib/$(STATIC)
	cp -f $(SHARED) /usr/local/lib/$(SHARED)
	cp -rf include/Socket /usr/local/include/cppsocket

uninstall:
	rm -f /usr/local/lib/$(STATIC)
	rm -f /usr/local/lib/$(SHARED)
	rm -rf /usr/local/include/cppsocket

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

