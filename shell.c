#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

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

void prompt(){
  // FILE *data = fopen("test.txt","rb");
  // if (data == NULL){
  //   printf("broken\n");
  //   err();
  //   return;
  // }
	char* temp;
  char input[1000]; // adjust
  printf("$ "); // mimic terminal
  fflush(stdout);
	temp = fgets(input, sizeof(input), stdin);
  if (temp == NULL){
    perror("could not get input\n");
    exit(1);
  }
	else if (strcmp(temp, "exit\n") == 0) {
		exit(1);
	}
  // scanf("%s",input);
	else {
		printf("%s",input);
	}
}

int main(){
  char input[1000];
  while (1){
    prompt();
  }
  return 0;
}
