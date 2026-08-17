// Just Investigate the behavior of read() on a closed file descriptor.

#include <fcntl.h>   // For open() and O_RDONLY
#include <unistd.h>  // For read(), close(), ssize_t

#include <cstdio>  // For printf() and perror()

int main() {
  // 1. Open "test.txt" in read-only mode
  int fd = open("test.txt", O_RDONLY);
  printf("fd = %d\n", fd);

  // 2. Immediately CLOSE the file descriptor
  close(fd);

  // 3. Attempt to READ 1 byte from the CLOSED file descriptor
  char c;
  ssize_t n = read(fd, &c, 1);

  // 4. Print the return value of read()
  printf("read returned = %zd\n", n);

  // 5. Investigate the system error state
  perror("read");

  return 0;
}

// It gonna print "read : Bad file descriptor" - because file descriptor is closed and read() cannot operate on it.