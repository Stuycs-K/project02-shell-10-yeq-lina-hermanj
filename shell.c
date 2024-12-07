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
  if (cmd == NULL){
    cmd = getenv("HOME");
  }
  if (chdir(cmd) != 0) {
    perror("cd failed");
  }
}

void execute(char * args[]){
  pid_t p = fork();
  if (p < 0){
    perror("fork fail");
    exit(1);
  }
  else if (p == 0){
    execvp(args[0], args);
    fprintf(stderr,"%s: command not found\n",args[0]);
    exit(1);
  }
  else{
    int n;
    wait(&n);
  }
}

void remove_newline(char * input){
  int len = strlen(input);
  if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
  }
}

int getinput(char* input, size_t size){
  printf("%s $ ",getcwd(input,size)); // mimic terminal
  fflush(stdout);
  char* temp = fgets(input, size, stdin);
  if (temp == NULL){
    if (feof(stdin)){
      printf("\n");
      return 0;
    }
  }
  remove_newline(input);
  return 1;
}

void prompt(){
  char* args[32];
	char* comd[32];
  char cwd[1024];
  char input[1024];
  char* temp;

  if (getcwd(cwd, sizeof(cwd)) == NULL){
    perror("could not get path");
    exit(1);
  }

  if (getinput(input, sizeof(input)) != 1){
    exit(1);
  }

  // remove \n
  int len = strlen(input);
	if (len > 0 && input[len - 1] == '\n') {
		input[len - 1] = '\0';
	}
	// copy to operate splicing on
  char cop[1024];
  strcpy(cop, input);
  char *copy = cop;

  //redirect parsing
  char c = ' ';
  char buff[1024];

  // semicolon
	//printf("num newline: %d\n", newline);
  int numcmds = 0;
  while ((comd[numcmds] = strsep(&copy,";")) != NULL){
    numcmds++;
  }
  for (int i = 0; i < numcmds; i++){
    c = check(comd[i]);
    // printf("%c\n", c);
    if (c == '0'){
      parse_args(comd[i], args);
    }
    else{
      strcpy(buff, parse_redirect(c, comd[i]));
      parse_args(buff, args);
      //change fd here to redirect
      //| requires a temp file
    }

    // printf("char: %c\n", c);
    //split | < > here and use that instead of comd[i]
    //writing in parse.c
    //takes a char arr, buffer and returns a char
    //returns |<>, writes part before into a buff and consumes that part of arr

    // parse_args(comd[i], args);
    if (strcmp(args[0],"exit") == 0){
      exit(0);
    }
    else if (strcmp(args[0],"cd") == 0){
      cd(args[1]);
    }
    else {
      execute(args);
    }
  }
}
