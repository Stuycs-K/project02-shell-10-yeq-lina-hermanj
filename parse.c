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
    if (strlen(arg_ary[c]) == 0) continue;
    c++;
  }
  arg_ary[c + 1] = NULL;
}

char check(char * line){
  int n = 0;
  char c = ' ';
  while(line[n] != '\0'){
    // printf("%d:", n);
    // printf("%c\n", line[n]);
    if (line[n] == '>' || line[n] == '<'){
      // printf("%s\n", "redirect");
      return(line[n]);
    }
    n++;
  }

  return '0';
}

char * parse_right(char c, char * line){
  char * temp = strcpy(temp, line);
  char * file;
  if (c == '>'){
    file = strchr(temp, '>') + 2;
  }
  else if (c == '<'){
    file = strchr(temp, '<') + 2;
  }
  else {
    return NULL;
  }
  // while (*file == ' ') file++;
  *strchr(line,c) = '\0';
  return file;


  // char * buffer;
  // char * sep = c == '>' ? ">" : (c == '<' ? "<" : ">>");
  // buffer = strsep(&temp, sep);
  // return temp;


  // char ch[4];
  // ch[0] = ' ';
  // ch[1] = c;
  // ch[2] = ' ';
  // ch[3] = '\0';
  // // printf("%c\n", ch[0]);
  // char * buff;
  // buff = strsep(&temp, ch);
  // // printf("buffer:%s\n", buff);
  // return buff;
}
char * parse_left(char c, char * line){
  char * temp = strcpy(temp, line);
  *(strchr(temp,c) - 1) = '\0';
  return temp;

}
