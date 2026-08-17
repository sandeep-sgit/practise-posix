// next q : now try to close(fd) in each iteration and see what happens.

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

int main() {
  while (1) {
    int fd = open("test.txt", O_RDONLY);

    if (fd == -1) {
      std::cerr << "Error opening file" << std::endl;
      return 1;
    }

    std::cout << "Opened file with fd: " << fd << std::endl;

    close(fd);  // Close the file descriptor after use
  }
}
