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

// void parse_args(char * line){
//   int argc = 1000, position = 0;
//   char ** arg_ary = malloc(argc * sizeof(char*));
//   char * erg;
//   if (!arg_ary){
//     fprintf(stderr, "allocation error\n");
//     exit(0);
//   }
//   char * temp = line;
//   int c = 0;
//   while((arg_ary[c] = strsep(&temp, ";"))){
//     printf("%s", arg_ary[c]);
//     c++;
//   }
//   arg_ary[c + 1] = NULL;
// }

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
  printf("%s $ ",cwd); // mimic terminal
  fflush(stdout);

  // cntrl + d check is here
  temp = fgets(input, sizeof(input), stdin);
  if (temp == NULL){
    if (feof(stdin)){
      printf("\n");
      exit(0);
    }
    perror("could not get input");
    exit(1);
  }
	
  // remove \n
  int len = strlen(input);
  if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
  }

	//check if textfile input
	int newline = 0;
	for (int i = 0; i < 1024; i++) {
		if (input[i] == '\n') {
			newline++;
		}
	}
	
  // copy to operate splicing on
  char cop[1024];
  strcpy(cop, input);
  char *copy = cop;

  // semicolon
  int numcmds = 0;
  while ((comd[numcmds] = strsep(&copy,";")) != NULL){
		if (newline > 0) {
			printf("%s\n", comd[numcmds]);
		}
    numcmds++;
  }
  for (int i = 0; i < numcmds; i++){
    parse_args(comd[i], args);
    if (strcmp(args[0],"exit") == 0){
      exit(0);
    }
    else if (strcmp(args[0],"cd") == 0){
      if (args[1] == NULL){
        cd(getenv("HOME"));
      }
      else {
        cd(args[1]);
      }
    }
    else {
      pid_t p;
      p = fork();
      if (p < 0){
        perror("fork fail");
        exit(1);
      }
      else if (p == 0){
        execvp(args[0], args);
        perror("execvp fail");
        exit(1);
      }
      else{
        int n;
        wait(&n);
      }
    }
  }
}
