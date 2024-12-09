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

void redirection(char c, char *file){
  int fd;
  if (c == '>'){
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
  }
  else if (c == '<'){
    fd = open(file, O_RDONLY);
  }
  else if (strcmp(&c, ">>") == 0){
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644 );
  }
  else {
    return;
  }
  if (fd < 0){
    perror("file open failed");
    exit(1);
  }
  if (c == '<'){
    dup2(fd, STDIN_FILENO);
  }
  else {
    dup2(fd, STDOUT_FILENO);
  }
  close(fd);
}

void pipefunc(char *commands[], int num_commands){
  char *temp = "temp";
  
  // write to temp
  char *arg1[100];
  parse_args(commands[0],arg1);
  pid_t p1 = fork();
  if (p1 == 0){
    int fd = open(temp, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
    dup2(fd, STDOUT_FILENO);
    close(fd);
    execvp(arg1[0],arg1);
    perror("execvp failed");
    exit(1);
  }
  else {
    wait(NULL);
  }

  // read from temp
  char *arg2[100];
  parse_args(commands[1],arg2);
  pid_t p2 = fork();
  if (p2 == 0){
    int fd = open(temp, O_RDONLY);
    dup2(fd, STDIN_FILENO);
    close(fd);
    execvp(arg2[0],arg2);
    perror("execvp failed");
    exit(1);
  }
  else {
    wait(NULL);
  }

  remove(temp);
  
  // int fd[2], in_fd = STDIN_FILENO;
  // for (int i = 0; i < num_commands; i++) {
  //   pipe(fd);
  //   pid_t p = fork();
  //   if (p == 0) {
  //     dup2(in_fd, STDIN_FILENO);
  //     if (i < num_commands - 1) {
  //       dup2(fd[1], STDOUT_FILENO);
  //     }
  //     close(fd[0]);
  //     char *args[32];
  //     parse_args(commands[i], args);
  //     execvp(args[0], args);
  //     perror("execvp failed");
  //     exit(1);
  //   } else if (p < 0) {
  //     perror("fork failed");
  //     exit(1);
  //   }
  //   close(fd[1]);
  //   in_fd = fd[0];
  //   wait(NULL);
  // }
}

void nospecial(char **args){
  pid_t pid = fork();
  if (pid == -1){
    perror("fork failed");
    return;
  }
  else if (pid == 0){
    execvp(args[0], args);
    perror("execvp");
    exit(1);
  }
  int status;
  wait(&status);
  return;
}

void prompt(){
  char* args[32];
	char ** cmds = malloc((sizeof(char)*1024)*1024);
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

    // // printf("%c\n", c);
    // if (c == '0'){
    //   parse_args(comd[i], args);
    // }
    // else{
    //   strcpy(buff, parse_redirect(c, comd[i]));
    //   parse_args(buff, args);
    //   //change fd here to redirect
    //   //| requires a temp file
    // }

    // printf("char: %c\n", c);
    //split | < > here and use that instead of comd[i]
    //writing in parse.c
    //takes a char arr, buffer and returns a char
    //returns |<>, writes part before into a buff and consumes that part of arr

    char * pipe_cmds[32];
    int num_pipes = 0;
    char *command = comd[i];

    // if | is in the cmd, then split up the commands into things to pipe
    if (strchr(command, '|')){
      while ((pipe_cmds[num_pipes] = strsep(&command,"|")) != NULL){
        num_pipes++;
      }
      pipefunc(pipe_cmds,num_pipes);
      continue;
    }
    // 
    else if (c == '>' || c == '<'){
      char *file = parse_redirect(c, command);
      parse_args(command,args);
      pid_t p = fork();
      if (p == 0){
        redirection(c, file);
        execvp(args[0], args);
        perror("exec failed");
        exit(1);
      }
      else {
        wait(NULL);
        continue;
      }
    }

    parse_args(comd[i], args);

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
