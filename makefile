compile: main.o parse.o shell.o
	@gcc -o shell main.o parse.o shell.o
main.o: main.c shell.h parse.h
	@gcc -c main.c shell.h parse.h
shell.o: shell.c shell.h parse.h
	@gcc -c shell.c shell.h parse.h
parse.o: parse.c shell.h parse.h
	@gcc -c parse.c shell.h parse.h
clean:
	@rm -f *.o shell
