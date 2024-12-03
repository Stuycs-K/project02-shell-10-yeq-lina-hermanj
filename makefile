compile: main.o parse.o shell.o
	@gcc -o shell main.o parse.o shell.o
main.o: main.c shell.h parse.h
	@gcc -c main.c 
shell.o: shell.c shell.h parse.h
	@gcc -c shell.c
parse.o: parse.c
	@gcc -c parse.c
clean:
	@rm -f *.o shell
