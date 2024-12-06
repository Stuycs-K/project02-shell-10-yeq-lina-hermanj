#ifndef PARSE_H
#define PARSE_H
void parse_args( char * line, char ** arg_ary );
char check(char * line);
char * parse_redirect(char c, char * line);
#endif
