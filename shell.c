#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "parse.h"

// int err(): prints out error message
int err(){
  printf("errno %d\n",errno);
  printf("%s\n",strerror(errno));
  exit(1);
}

// void cd(char * cmd): takes a char* cmd, checks if has no args (sends to home) or args (takes to directed path)
// char* cmd: the path
void cd(char * cmd) {
  if (cmd == NULL){
    cmd = getenv("HOME");
  }
  if (chdir(cmd) != 0) {
    perror("cd failed");
  }
}

// void execute(char * args[]): takes a char* args[], forks and execs child
// char* args[]: args for command to exec
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

// void remove_newline(char * input): takes a char* input, removes newline from input
// char* input: input line to change
void remove_newline(char * input){
  int len = strlen(input);
  if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
  }
}

// int getinput(char* input, size_t size): takes a char* input, size_t size. mimics terminal, takes userinput, and checks for cntrl+d. returns 1 if succeeds, 0 if not
// char* input: buffer to store input
// size_t size: buffer size
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

// int pipe_count(char *args): takes a char* args, and counts the number of pipes
// char *args: array of arguments in the input
int pipe_count(char *args){
  int count = 0;
  for (int i = 0; i < strlen(args); i++){
    count += (args[i] == '|');
  }
  return count;
}

// void redirection(char **comd, char *input, char *output, int append_flag): for < > >>, assigns proper file perms, then forks and execvps proper operation, restores file descriptors after
// char **comd: the command to execute
// char *input: the input file name
// char *output: the output file name
// int append_flag: 1 for >>, 0 for > and <
void redirection(char **comd, char *input, char *output, int append_flag){
  int flags;
  if (append_flag = 1){
    flags = (O_RDWR | O_CREAT | O_APPEND);
  }
  else {
    flags = (O_RDWR | O_CREAT);
  }
  // fds
  int stdin = dup(0);
  int stdout = dup(1);
  int fd_in = open(input, O_RDONLY);
  int fd_out = open(output, flags, 0644);
  if (fd_in == -1){
    perror("open input failed");
    return;
  }
  if (fd_out == -1){
    perror("open output failed");
    return;
  }
  dup2(fd_in,0);
  dup2(fd_out,1);
  // fork and execvp
  pid_t pid = fork();
  if (pid == -1){
    perror("fork failed");
    return;
  }
  else if (pid == 0){
    execvp(comd[0],comd);
    perror("execvp failed");
    exit(1);
  }
  int status;
  wait(&status);
  // clean
  dup2(stdin,0);
  dup2(stdout,1);
  close(stdin);
  close(stdout);
  close(fd_in);
  close(fd_out);
  return;
}

// void less_than(char **comd, char *input): redirects input from a file.
// redirects stdin into input file, forks + execs, restores stdin
// char **comd: the command to execute
// char *input: the input file name
void less_than(char **comd, char *input) {
	int stdin = dup(0);
  int fd = open(input, O_RDONLY);
  if (fd == -1){
    perror("cannot open file");
    return;
  }
  dup2(fd, 0);
  pid_t pid = fork();
  if (pid == -1){
    perror("cannot fork");
    return;
  }
  else if (pid == 0){
    execvp(comd[0],comd);
    perror("execvp failed");
    exit(1);
  }
  int status;
  wait(&status);
  dup2(stdin, 0);
  close(stdin);
  close(fd);
  return;
}

// void greater_than(char **comd, char *output, int append_flag): handles output redirection (>)
// redirects stdout into output file, forks + execs, restores stout
// char **comd: the command to execute
// char *input: the input file name
// int append_flag: 1 for append (>>), 0 for overwrite (>)
void greater_than(char **comd, char *output, int append_flag) {
  int flags;
  int stdout = dup(1);
  if (append_flag == 1){
    flags = (O_RDWR | O_CREAT | O_APPEND);
  }
  else {
    flags = (O_RDWR | O_CREAT);
  }
	int fd = open(output, flags, 0644);
  if (fd == -1){
    perror("cannot open file");
    return;
  }
	dup2(fd, 1);
	pid_t pid = fork();
  if (pid == -1){
    perror("fork failed");
  }
  else if (pid == 0){
    execvp(comd[0], comd);
    perror("execvp failed");
    exit(1);
  }
  int status;
  wait(&status);
  dup2(stdout, 1);
  close(stdout);
  close(fd);
  return;
}

// void pipefunc(char **comd, int num_commands): handles pipes
// calls pipe() for each pipe, forks + execs, connects stdin with fd[0] for all except first comd + stdout with fd[1] for all except last comd
// char **comd: each segment of the pipe; array of commands to exec
// int num_commands: numbers of commands in the pipe
void pipefunc(char **comd, int num_commands){ 
  int fd[2], infd = STDIN_FILENO;
  for (int i = 0; i <= num_commands; i++) {
    if (pipe(fd) == -1){
      perror("pipe failed");
      exit(1);
    }
    pid_t p = fork();
    if (p == 0) {
      if (i != 0) {
        dup2(infd, 0);
      }
      if (i != num_commands){
        dup2(fd[1], 1);
      }
      parse_redir(comd[i]);
      exit(1);
    } 
    else if (p < 0) {
      perror("fork failed");
      exit(1);
    }
    else {
      // store fd[0] for the next iterate
      int n;
      wait(&n);
      infd = fd[0];
      close(fd[1]);
    }
  }
}

// void prompt(): displays the prompt, processes user input
// shows directory, processes input, parses input, executes with children
void prompt(){
	char *cmds[1000]; // array for semicolon seperated commands
  char cwd[1024]; // curr working directory buff
  char input[1024]; // input buffer

  // curr working directory
  if (getcwd(cwd, sizeof(cwd)) == NULL){
    perror("could not get path");
    exit(1);
  }

  // prompt for input
  if (getinput(input, sizeof(input)) != 1){
    exit(1);
  }

  // remove new line
  remove_newline(input);

  // enter case
  if (strlen(input) == 0){
    return;
  }

  // exit case
  if (strcmp(input,"exit") == 0){
    exit(0);
  }

  // split input into commands by semicolon
  char cop[1024];
  strcpy(cop, input);
  char *copy = cop;

  // semicolon
  int numcmds = 0;
  while ((cmds[numcmds] = strsep(&copy,";")) != NULL){
    numcmds++;
  }
  for (int i = 0; i < numcmds; i++){
    if (strchr(cmds[i],'|') || strchr(cmds[i],'<') || strchr(cmds[i],'>') ){
      parse_pipe(cmds[i]);
    }
    else {
      char* args[100]; // array to store command args
      strtok(input, "\n");
      parse_args(cmds[i],args);
      if (strcmp(args[0],"cd") == 0){
        cd(args[1]);
      }
      else {
        execute(args);
      }
    }
  }
}
