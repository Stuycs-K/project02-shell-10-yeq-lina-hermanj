[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell
We are: Shell Shockers

Joy Herman, Abigail Lin, Leo Ye

## Implemented
1. Semicolon separated commands
2. cd
3. Prompt (not shortened)
4. exit and ^d to exit shell
5. ./shell < lines.txt
   - but it prints next to the prompt
6. redirects but not chained together

## Attempted to Implement
1. combining redirects and pipes together


## Function Headers
int err(){}
- for errno

void cd(char * cmd) {}
- cd

void remove_newline(char * input){}
- removes the new line

int getinput(char* input, size_t size){}
- takes the input

char hasPipe(char * line){}
- checks for pipe

void prompt(){}
- main logic, runs everything. parses and execs in here

char * parse_right(char c, char * line){}
- takes inputs with redirects and returns a pointer to the right side of the redirect and adds a terminating 0 to the end of the left.

void parse_args( char * line, char ** arg_ary ){}
- parses the input into an arg array for execvp

char check(char * line){}
- checks for redirects and returns the type of redirect
