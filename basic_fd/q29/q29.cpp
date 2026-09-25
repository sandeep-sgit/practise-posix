// Write program to open test.txt and read one character at a time in loop

#include <fcntl.h>   // For open() and O_RDONLY
#include <limits.h>  // For PATH_MAX
#include <unistd.h>  // For read(), close(), ssize_t

#include <iostream>

int main() {
  int fd = open("test.txt", O_RDONLY);
  char c;

  while (1) {
    ssize_t n = read(fd, &c, 1);

    if (n == 0) break;  // End of file

    if (n == -1) {
      perror("read");
      close(fd);
      return 1;
    }

    std::cout << "Character read: " << c << std::endl;
  }
  return 0;
}