// Write program which opens test.txt with 2 fds initiatlly (fd1, fd2) - prints the fds given by OS
// then closes the fd1
// then opens same file with fd3, and prints the fds given by OS.

/* Observe- does the fd3 no is same which we closed fd1?
            Can I close standard fds (like stdin, stdout, stderr)
            what if I closed stdin here instead of fd1
            Does linux allots lowest fd first?
*/

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

int main() {
  int fd1 = open("test.txt", O_RDONLY);
  int fd2 = open("test.txt", O_RDONLY);

  std::cout << "fd1: " << fd1 << std::endl;
  std::cout << "fd2: " << fd2 << std::endl;

  close(fd1);

  int fd3 = open("test.txt", O_RDONLY);
  std::cout << "fd3: " << fd3 << std::endl;

  return 0;
}