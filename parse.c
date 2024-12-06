#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "shell.h"

void parse_args( char * line, char ** arg_ary ){
  char * temp = line;
  int c = 0;
  while((arg_ary[c] = strsep(&temp, " "))){
    // printf("%s\n", arg_ary[c]);
    c++;
  }
  arg_ary[c + 1] = NULL;
}

char parse_redirect(char * line, char * buffer){
  char * temp = line;
  int n = 0;
  char c = ' ';
  while(line[n] != '\0'){
    printf("%d:", n);
    printf("%c\n", line[n]);
    if (line[n] == '|' || line[n] == '>' || line[n] == '<'){
      printf("%s\n", "redirect");
      return(line[n]);
    }
    n++;
  }
  
  return '\0';
}
