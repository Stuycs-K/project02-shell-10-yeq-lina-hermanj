#ifndef SHELL_H
#define SHELL_H
void prompt();
void cd(char * cmd);
int err();
void execute(char * args[]);
void remove_newline(char * input);
int getinput(char* input, size_t size);
void redirection(char c, char *file);
void less_than(char **comd, char *input);
void greater_than(char **comd, char *output, int append_flag);
void pipefunc(char **comd, int num_commands);
void nospecial(char **args);
#endif
