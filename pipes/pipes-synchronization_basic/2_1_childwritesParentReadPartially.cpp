// Write program in which child waits for 5 sec (observe read from parent blocks) and then child writes "Hello message from child" to pipe
// and parent reads partially only 2 bytes and prints it to terminal

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
    // close(fd[1]); // Close write end after writing
  } else {  // Parent process
    char buffer[100];
    cout << "Parent is waiting to read from pipe..." << endl;
    ssize_t bytesRead = read(fd[0], buffer, 2);  // Read only 2 bytes from the pipe (partial read from pipe data)

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

// Observe that I commented out close() : closing the write end of pipe is importnat other wise read() will block forever. (see in next program)
// here its working becuase child process goes to exit & kernel closes write end of pipe automatically. But if child process is still running then read() will block forever. (see next program)