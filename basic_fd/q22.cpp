// write a program to print the file descriptors number of these process

#include <unistd.h>

#include <iostream>

int main() {
  printf("FD number of stdin: %d\n", STDIN_FILENO);
  printf("FD number of stdout: %d\n", STDOUT_FILENO);
  printf("FD number of stderr: %d\n", STDERR_FILENO);

  return 0;
}