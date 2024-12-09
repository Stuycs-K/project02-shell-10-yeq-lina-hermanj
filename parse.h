#ifndef PARSE_H
#define PARSE_H
void parse_args( char * line, char ** arg_ary );
char check(char * line);
char * parse_right(char c, char * line);
char * parse_left(char c, char * line);
#endif
