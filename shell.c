#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parse.h"

int err(){
  printf("errno %d\n",errno);
  printf("%s\n",strerror(errno));
  exit(1);
}

void cd(char * cmd) {
  if (chdir(cmd) != 0) {
    perror("cd failed");
  }
}

void prompt(){
  char* temp;
  char * args[32];
  char* comd;
  //chdir(getenv("HOME"));
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) == NULL){
    perror("could not get path");
    exit(1);
  }
  printf("%s $ ",cwd); // mimic terminal
  char input[1024]; // adjust
  fflush(stdout);
  temp = fgets(input, sizeof(input), stdin);


  // parses the input
  char copy[1024];
  strcpy(copy, input);
  int len = strlen(copy);
  if (len > 0 && copy[len - 1] == '\n') {
      copy[len - 1] = '\0';
  }
  parse_args(copy, args);

  if (temp == NULL){
    if (feof(stdin)){
      printf("\n");
      exit(0);
    }
    perror("could not get input");
    exit(1);
  }
  else if (strcmp(temp, "exit\n") == 0) {
    exit(1);
  }
  comd = strsep(&temp, " ");
  temp = strsep(&temp, "\n");
  if (strcmp(comd, "cd") == 0) {
    cd(temp);
  }
  else {
    // printf("%s\n",input);
    // execvp(args[0], args);

    pid_t p;
    p = fork();
    if (p < 0){
      exit(1);
    }
    else if (p == 0){
      execvp(args[0], args);
    }
    else{
      int n;
      wait(&n);
    }
  }
}
