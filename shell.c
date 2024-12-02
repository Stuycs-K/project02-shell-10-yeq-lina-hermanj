#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int err(){
  printf("errno %d\n",errno);
  printf("%s\n",strerror(errno));
  exit(1);
}

void read_csv(){
  FILE *data = fopen("test.txt","rb");
  if (data == NULL){
    printf("broken\n");
    err();
    return;
  }
}

void prompt(){
  FILE *data = fopen("test.txt","rb");
  if (data == NULL){
    printf("broken\n");
    err();
    return;
  }
  char input[100]; // adjust
  scanf("%s",input);
  printf("%s",input);
}

int main(){
  prompt();
}
