all: main

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ "$(OBJ_DIR)"

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ "$(OBJ_DIR)"


main: 
	#$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f *.exe
