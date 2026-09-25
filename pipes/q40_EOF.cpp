/* Write program in which parent sends data to child thru pipe and child reads and prints back to terminal untill parent sends EOF.*/

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int main() {
  int fd[2];
  if (pipe(fd) == -1) {
    cerr << "Pipe creation failed" << endl;
    return 1;
  }

  pid_t pid = fork();

  if (pid < 0) {
    cerr << "Fork failed" << endl;
    return 1;
  }

  if (pid == 0) {  // Child process
    close(fd[1]);  // Close unused write end
    char buffer[4096];

    while (true) {
      ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);
      if (bytesRead <= 0) {
        cout << "\nChild received EOF or error" << endl;
        break;
      }
      buffer[bytesRead] = '\0';
      cout << "Child received message: " << buffer << endl;
    }

    close(fd[0]);  // Close read end
    exit(0);

  } else {         // Parent process
    close(fd[0]);  // Close unused read end
    string message;

    while (true) {
      cout << "Enter message (or Ctrl+D for EOF): ";
      if (!getline(cin, message)) {
        write(fd[1], "", 0);  // Send EOF to child
        break;                // Break when user triggers EOF on cin
      }

      write(fd[1], message.c_str(), message.length());
      usleep(50000);  // Pause for 0.2 seconds (200,000 microseconds)
    }

    close(fd[1]);  // Closing the write pipe automatically sends EOF to child
    wait(NULL);    // Wait for child process to finish
  }

  return 0;
}