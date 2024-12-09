#ifndef SHELL_H
#define SHELL_H
void prompt();
void cd(char * cmd);
int err();
void remove_newline(char * input);
void execute(char * args[]);
#endif
