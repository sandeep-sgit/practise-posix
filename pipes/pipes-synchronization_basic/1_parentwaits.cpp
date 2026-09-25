// Write an program to understand after creating pipe if you read then it waits

#include <unistd.h>

#include <iostream>
using namespace std;

int main() {
  int fd[2];
  if (pipe(fd) == -1) {
    perror("pipe");
    return 1;
  }

  char buffer[100];
  cout << "Reading from pipe (this will block until data is written)..." << endl;
  ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);

  if (bytesRead == -1) {
    perror("read");
    return 1;
  }

  return 0;
}