#ifndef PARSE_H
#define PARSE_H
void parse_args( char * line, char ** arg_ary );
char checkorder(char * line);
void parse_pipe(char *input);
void parse_redir(char *input);
#endif
