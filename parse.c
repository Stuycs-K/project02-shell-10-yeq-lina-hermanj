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

char check(char * line){
  int n = 0;
  char c = ' ';
  while(line[n] != '\0'){
    // printf("%d:", n);
    // printf("%c\n", line[n]);
    if (line[n] == '|' || line[n] == '>' || line[n] == '<'){
      // printf("%s\n", "redirect");
      return(line[n]);
    }
    n++;
  }

  return '0';
}

char * parse_redirect(char c, char * line){
  char * temp = line;
  char ch[4];
  ch[0] = ' ';
  ch[1] = c;
  ch[2] = ' ';
  ch[3] = '\0';
  // printf("%c\n", ch[0]);
  char * buff;
  buff = strsep(&temp, ch);
  // printf("buffer:%s\n", buff);
  return buff;
}

void greater_than(char * file, char * args[]) {
	int fd1 = open(file, O_WRONLY);
	int stdout = STDOUT_FILENO;
	int backup_stdout = dup( stdout );
	dup2(fd1, stdout);
	execute(args);
	//fflush(stdout);
	dup2(backup_stdout, stdout);
}
