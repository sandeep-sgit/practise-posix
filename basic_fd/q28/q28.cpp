// Write program which open "test.txt" file and read the one character from it and print it on the screen.
// Observe where does rest "BCDEFG" is - its in kernel ram? or disk? or in fd?

#include <fcntl.h>   // For open() and O_RDONLY
#include <limits.h>  // For PATH_MAX
#include <unistd.h>  // For read(), close(), ssize_t
#include <unistd.h>  // For getcwd()

#include <cstdio>  // For printf() and perror()
#include <iostream>
#include <string>  // For std::string

int main() {
  // 1. Open "test.txt" in read-only mode
  int fd = open("test.txt", O_RDONLY);

  if (fd == -1) {
    perror("open");
    return 1;
  }

  // 2. Read one character from the file
  char c;
  ssize_t n = read(fd, &c, 1);

  if (n == -1) {
    perror("read");
    close(fd);
    return 1;
  }

  // 3. Print the character read
  std::cout << "Character read: " << c << std::endl;

  // print current working directory for DEBUG
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != nullptr) {
    std::cout << "Current working directory: " << cwd << std::endl;
  } else {
    perror("getcwd");
  }
}