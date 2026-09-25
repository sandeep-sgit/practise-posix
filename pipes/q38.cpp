/* Write program to feel PIPE- as a Byte Stream
    Program simply takes message from user in form of argument and direct it to PIPE write end and from another end
    program read the message and print it to terminal */

// Mechanism- Directly connect stdin--->Pipe write end---->Pipe read end---->stdout

#include <unistd.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char* argv[]) {
  int fd[2];
  pipe(fd);

  // I not gonna use buffer here, I gonna use dup2()
  exit(0);

}

//still we needed buffer,