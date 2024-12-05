#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

void parse_args( char * line, char ** arg_ary ){
  char * temp = line;
  int c = 0;
  while((arg_ary[c] = strsep(&temp, " "))){
    // printf("%s\n", arg_ary[c]);
    c++;
  }
  arg_ary[c + 1] = NULL;
}