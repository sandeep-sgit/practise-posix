// Program in which child writes to pipe end but waits for an minute showing that parent read end is blocked untill child process closes write end of pipe or sends eof to write end.

#include <unistd.h>

#include <iostream>

using namespace std;

int main() {
  int fd[2];
  if (pipe(fd) == -1) {
    perror("pipe");
    return 1;
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {  // Child process
    sleep(5);      // Wait for 5 seconds
    const char* message = "Hello message from child";
    write(fd[1], message, strlen(message));
    sleep(60);     // Wait for 1 minute before closing the write end & observe parent also blocked on read() end
    close(fd[1]);  // Close write end after writing
  } else {         // Parent process
    char buffer[100];
    cout << "Parent is waiting to read from pipe..." << endl;
    ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);

    if (bytesRead == -1) {
      perror("read");
      return 1;
    }

    buffer[bytesRead] = '\0';  // Null-terminate the string
    cout << "Parent received: " << buffer << endl;
    // close(fd[0]); // Close read end after reading
  }

  return 0;
}