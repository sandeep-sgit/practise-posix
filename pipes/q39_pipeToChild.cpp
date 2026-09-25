/* write program which takes message as arg and the program gonna fork child and
send message to child via pipe and child gonna print the message to terminal*/

#include <unistd.h>

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
  int fd[2];
  pipe(fd);

  pid_t pid = fork();
  if (pid == 0) {  // Child process
    close(fd[1]);  // Close write end of the pipe in child
    char buffer[4096];
    int bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';
    cout << "Child received message: " << buffer << endl;
    close(fd[0]);  // Close read end of the pipe in child
  } else {         // Parent process
    close(fd[0]);  // Close read end of the pipe in parent
    string message = "";
    for (int i = 1; i < argc; i++) {
      message += argv[i];
      message += " ";
    }
    sleep(5); //Child also waits here - child process gonna waiting/sleeping from kernel side.
    write(fd[1], message.c_str(), message.size());
    close(fd[1]);  // Close write end of the pipe in parent
  }
  return 0;
}