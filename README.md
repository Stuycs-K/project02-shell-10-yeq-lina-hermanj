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
6. redirects
7. pipes

## Attempted to Implement
1. ./shell < lines.txt doesn't work


## Function Headers
int err(){}
- prints out error message

void cd(char * cmd) {}
- checks if has no args (sends to home) or args (takes to directed path)

void execute(char * args[]){}
- forks and execs child

void remove_newline(char * input){}
- removes the new line

int getinput(char* input, size_t size){}
- mimics terminal, takes userinput, and checks for cntrl+d. returns 1 if succeeds, 0 if not

int pipe_count(char *args){}
- counts the number of pipes

void redirection(char **comd, char *input, char *output, int append_flag){}
- handles redirection < > >>
- assigns proper file perms, then forks and execvps proper operation, restores file descriptors after

void less_than(char **comd, char *input){}
- redirects input from a file

void greater_than(char **comd, char *output, int append_flag){}
- handles output redirection (>)

void pipefunc(char **comd, int num_commands){}
- handles pipes

void prompt(){}
- main logic, runs everything. parses and execs in here

void parse_args( char * line, char ** arg_ary ){}
- breaks apart a line into an array of arguments

char checkorder(char * line){}
- finds the first > or <

void parse_redir(char *input){}
- parses redirection (> >> <) and calls each func to exec it

void parse_pipe(char *input){}
- parses pipe and calls pipefunc to exec it
