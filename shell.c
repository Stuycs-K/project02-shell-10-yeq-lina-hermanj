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

void parse_args( char * line, char ** arg_ary ){
  char * temp = line;
  int c = 0;
  while((arg_ary[c] = strsep(&temp, " "))){
    printf("%s\n", arg_ary[c]);
    c++;
  }
  arg_ary[c + 1] = NULL;
}

void cd(char * cmd) {
  if (chdir(cmd) != 0) {
    perror("cd failed");
  }
}

void prompt(){
	char* temp;
  char ** args;
  chdir(getenv("HOME"));
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) == NULL){
    perror("could not get path");
    exit(1);
  }
  char input[1024]; // adjust
	temp = fgets(input, sizeof(input), stdin);
  if (temp == NULL){
    if (feof(stdin)){
      printf("exit (end of file)\n");
      exit(1);
    }
    perror("could not get input");
    exit(1);
  }
	else if (strcmp(temp, "exit\n") == 0) {
		exit(1);
	}
  else if (strcmp(temp, "cd\n") == 0) {
    cd("..");
  }
	else {
		printf("%s",input);
	}
  printf("%s $ ",cwd); // mimic terminal
  fflush(stdout);
}

int main(){
  char input[1000];
  while (1){
    prompt();
  }
  return 0;
}
