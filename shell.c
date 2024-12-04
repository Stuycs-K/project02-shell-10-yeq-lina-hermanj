#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

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
  char ** args;
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
		printf("%s\n",input);
	}
}
