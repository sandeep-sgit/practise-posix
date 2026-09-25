/* Write program to feel PIPE- as a Byte Stream
    Program simply takes message from user in form of argument and direct it to PIPE write end and from another end
    program read the message and print it to terminal */

// Mechanism- Program uses String space to hold messge

#include <string.h>
#include <unistd.h>

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
  int fd[2];
  pipe(fd);

  // converting all argumnts to one string
  string message = "";
  for (int i = 1; i < argc; i++) {
    message += argv[i];
    message += " ";
  }

  // write message to pipe
  write(fd[1], message.c_str(), message.size());

  char buffer[4096];
  int bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);
  buffer[bytesRead] = '\0';
  string readMessage = buffer;
  cout << readMessage << endl;
  exit(0);
}