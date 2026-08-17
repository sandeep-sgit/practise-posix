// See what open gives you

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

int main() {
  int fd1 = open("test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  int fd2 = open("test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  std::cout << "FD1 :  " << fd1 << std::endl;
  std::cout << "FD2 :  " << fd2 << std::endl;

  close(fd1);
  close(fd2);

  return 0;
}