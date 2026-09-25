// Try closing standard fds and see what happen - run command "lsof -p pid" to see the fds opened by the process

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

int main() {
  std::cout << "PID : " << getpid() << std::endl;

  close(0);  // closing stdin
  close(1);  // closing stdout
  close(2);  // closing stderr

  int fd1 = open("test.txt", O_RDONLY);
  int fd2 = open("test.txt", O_RDONLY);

  std::cout << "fd1: " << fd1 << std::endl;
  std::cout << "fd2: " << fd2 << std::endl;

  std::cout << "I am going to sleep for 45 seconds - run command 'lsof -p pid' to see the fds opened by the process" << std::endl;
  sleep(45);

  return 0;
}