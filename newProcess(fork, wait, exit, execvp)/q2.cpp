// Write program which creates another process (child) --> that specifically
// prints child PID & Parent gonna prints it's own PID

#include <iostream>
#include <unistd.h>

int main() {
  std ::cout << "Hi, Sandeep - !!!WelcomeBack!!! " << getpid() << std::endl;

  int pid = fork(); // Creating a new process

  if (pid == 0) { // This block is executed by the child process
    std::cout << "This is the child process. Child PID: " << getpid()
              << std::endl;
  } else if (pid > 0) { // This block is executed by the parent process
    std::cout << "This is the parent process. Parent PID: " << getpid()
              << std::endl;
  }

  std ::cout << "PID received when Forked - " << pid << std::endl;
}