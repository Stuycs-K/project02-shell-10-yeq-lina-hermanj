#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
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


char hasPipe(char * line){
  int n = 0;
  char c = ' ';
  while(line[n] != '\0'){
    if (line[n] == '|'){
      return(line[n]);
    }
    n++;
  }
  return '0';
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
  remove_newline(input);
  char cop[1024];
  strcpy(cop, input);
  char *copy = cop;
  char c = ' ';
  char buff[1024];
  int numcmds = 0;
  while ((comd[numcmds] = strsep(&copy,";")) != NULL){
    numcmds++;
  }
  char * file;
  int fd = 0;
  int backup = 0;
  int currFd = 0;
  char p;
  for (int i = 0; i < numcmds; i++){
    c = check(comd[i]);
      p = hasPipe(comd[i]);
      if (p == '|'){
        int tempFd = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        int b = dup(STDOUT_FILENO);
        int cFd = STDOUT_FILENO;
        dup2(tempFd, STDOUT_FILENO);
        char * str = strchr(comd[i], '|') + 2;
        *(strchr(comd[i], '|') - 1) = '\0';
        parse_args(comd[i], args);
          pid_t p = fork();
          if (p == 0){
            execvp(args[0], args);
            fprintf(stderr,"%s: command not found\n",args[0]);
            exit(1);
          }
          else {
            wait(NULL);
            dup2(b, cFd);
          }
          tempFd = open("temp", O_RDONLY);
          b = dup(STDIN_FILENO);
          cFd = STDIN_FILENO;
          dup2(tempFd, STDIN_FILENO);
          // printf("%s\n", str);
          parse_args(str, args);
            p = fork();
            if (p == 0){
              execvp(args[0], args);
              fprintf(stderr,"%s: command not found\n",args[0]);
              exit(1);
            }
            else {
              wait(NULL);
              dup2(b, cFd);
              continue;
            }
          }

  //   p = hasPipe(comd[i]);
  //   if (p == '|'){
  //     int tempFd = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  //     int b = dup(STDOUT_FILENO);
  //     int cFd = STDOUT_FILENO;
  //     dup2(tempFd, STDOUT_FILENO);
  //
  //     char * temp = comd[i];
  //     char * str2;
  //     str2 = comd[i];
  //     comd[i] = strchr(comd[i], '|') + 2;
  //     *(comd[i] - 3) = '\0';
  //     char c1 = 0;
  //     char c2 = 0;
  //     if ((c1 = check(str2))){
  //       file = strchr(str2, '<') + 2;
  //       *(strchr(str2, '<') - 1) = '\0';
  //         fd = open(file, O_RDONLY);
  //         printf("%s\n", file);
  //         printf("%s\n", file);
  //         if (fd < 0){
  //           perror("file open failed");
  //           exit(1);
  //         }
  //         backup = dup(STDIN_FILENO);
  //         currFd = STDIN_FILENO;
  //         dup2(fd, STDIN_FILENO);
  //         close(fd);
  //       }
  //       parse_args(str2, args);
  //       pid_t p = fork();
  //       if (p == 0){
  //         execvp(args[0], args);
  //         fprintf(stderr,"%s: command not found\n",args[0]);
  //         exit(1);
  //       }
  //       else {
  //         wait(NULL);
  //         dup2(backup, currFd);
  //         dup2(b, cFd);
  //         // continue;
  //   }
  //
  //
  //   b = dup(STDIN_FILENO);
  //   cFd = STDIN_FILENO;
  //   dup2(tempFd, STDIN_FILENO);
  //   if ((c2 = check(comd[i]))){
  //     file = strchr(comd[i], '>') + 2;
  //     *(strchr(comd[i], '>') - 1) = '\0';
  //     // printf("%s\n", comd[i]);
  //       fd = open(file, O_RDONLY);
  //       if (fd < 0){
  //         perror("file open failed");
  //         exit(1);
  //       }
  //       backup = dup(STDOUT_FILENO);
  //       currFd = STDOUT_FILENO;
  //       dup2(fd, STDOUT_FILENO);
  //       close(fd);
  //     }
  //     parse_args(comd[i], args);
  //     // printf("%s\n", comd[i]);
  //     p = fork();
  //     if (p == 0){
  //       execvp(args[0], args);
  //       fprintf(stderr,"%s: command not found\n",args[0]);
  //       exit(1);
  //     }
  //     else {
  //       wait(NULL);
  //       dup2(backup, currFd);
  //       dup2(b, cFd);
  //       continue;
  // }
  //   // exit(0);
  // }


    else if (c == '>' || c == '<'){
      if (c == '>'){
        file = parse_right(c, comd[i]);
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
        if (fd < 0){
          perror("file open failed");
          exit(1);
        }
        backup = dup(STDOUT_FILENO);
        currFd = STDOUT_FILENO;
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
      else{
        file = parse_right(c, comd[i]);
        fd = open(file, O_RDONLY);
        if (fd < 0){
          perror("file open failed");
          exit(1);
        }
        backup = dup(STDIN_FILENO);
        currFd = STDIN_FILENO;
        dup2(fd, STDIN_FILENO);
        close(fd);
         // comd[i] = strchr(comd[i], c) + 2;
      }
    }

    parse_args(comd[i],args);

    if (strcmp(args[0],"exit") == 0){
      exit(0);
    }
    else if (strcmp(args[0],"cd") == 0){
      cd(args[1]);
    }
    else {

      pid_t p = fork();
      if (p == 0){
        // redirection(c, file);
        execvp(args[0], args);
        fprintf(stderr,"%s: command not found\n",args[0]);
        exit(1);
      }
      else {
        wait(NULL);
        dup2(backup, currFd);
        continue;
      }
    }
  }
}


// char * pipe_cmds[32];
// int num_pipes = 0;
// char *command = comd[i];
//
// if (strchr(command, '|')){
//   while ((pipe_cmds[num_pipes] = strsep(&command,"|")) != NULL){
//     num_pipes++;
//   }
//   pipefunc(pipe_cmds,num_pipes);
//   continue;
// }
// else if (c == '>' || c == '<'){
//   char *file = parse_redirect(c, command);
//   parse_args(command,args);
//   pid_t p = fork();
//   if (p == 0){
//     redirection(c, file);
//     execvp(args[0], args);
//     perror("exec failed");
//     exit(1);
//   }
//   else {
//     wait(NULL);
//     continue;
//   }
// }
