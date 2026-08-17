// See what open() actually gives you

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

int main() {
  int fd = open("test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  std::cout << "FD :  " << fd << std::endl;

  close(fd);

  return 0;
}