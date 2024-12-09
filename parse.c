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

char checkorder(char * line){
  int first;
  for (int i = 0; i < strlen(line); i++){
    if (line[i] == '<' || line[i] == '>'){
      first = line[i];
      break;
    }
  }
  return (char)first;
  // int n = 0;
  // char c = ' ';
  // while(line[n] != '\0'){
  //   // printf("%d:", n);
  //   // printf("%c\n", line[n]);
  //   if (line[n] == '|' || line[n] == '>' || line[n] == '<'){
  //     // printf("%s\n", "redirect");
  //     return(line[n]);
  //   }
  //   n++;
  // }

  // return '0';
}

// char * parse_redirect(char c, char * line){
//   char * temp = line;
//   char * file;
//   if (c == '>'){
//     file = strchr(temp, '>') + 1;
//   }
//   else if (c == '<'){
//     file = strchr(temp, '<') + 1;
//   }
//   else {
//     return NULL;
//   }
//   while (*file == ' ') file++;
//   *strchr(temp,c) = '\0';
//   return file;

//   // char * buffer;
//   // char * sep = c == '>' ? ">" : (c == '<' ? "<" : ">>");
//   // buffer = strsep(&temp, sep);
//   // return temp;


//   // char ch[4];
//   // ch[0] = ' ';
//   // ch[1] = c;
//   // ch[2] = ' ';
//   // ch[3] = '\0';
//   // // printf("%c\n", ch[0]);
//   // char * buff;
//   // buff = strsep(&temp, ch);
//   // // printf("buffer:%s\n", buff);
//   // return buff;
// }

void parse_redir(char *input){
  char *arg;
  char **args = malloc(4096);
  int flag;
  int append_flag = 0;
  int order_flag = 0;
  int count = 0;
  if ((strchr(input, '<')) || (strstr(input, ">"))) {
    // check for append flag
    if (strstr(input, " >> ")){
      append_flag = 1;
    }
    // input redir
    if (strchr(input, '<')){
      while ((arg = strtok_r(input, "<", &input))){
        args[count] = arg;
        count++;
      }
      flag = 1;
    }
    // output redir
    else if (strstr(input, ">")) {
      while ((arg = strtok_r(input, ">", &input))){
        args[count] = arg;
        count++;
      }
      flag = 0;
    }
    count = 0;
    char *comdarg;
    char **comdargs = malloc(4096);
    while ((comdarg = strtok_r(args[0], " ", &args[0]))){
      comdarg[count] = comdarg;
      count++;
    }
    char *file = strtok(args[1], " ");
    // call funcs based on each flag
    if (flag == 0){
      greater_than(comdargs,file,append_flag);
    }
    else if (flag == 1){
      less_than(comdargs,file);
    }
    free(comdarg);
  }
  // no special
  else {
    count = 0;
    while ((arg = strtok_r(input, " ", &input))){
      args[count] = arg;
      count++;
    }
    nospecial(args);
  }
  free(args);
  return;
}

void parse_pipe(char *input){
  char *arg;
  char **args = malloc(4096);
  int count;
  // first, parse for pipes. if none, then parse for redir
  if (strchr(input, '|')){
    int num_pipes = count_pipes(input);
    count = 0;
    while((arg = strtok_r(input, "|", &input))) {
        args[count] = arg; 
        count++; 
    }
    handle_pipes(args,num_pipes);
    free(args);
  }
  else {
    parse_redir(input);
  }
  return;
}