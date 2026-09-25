// Write an program in which producer sends message to consumer with priority.
// Consumer gonna receive message here instantly so start consumer process after sending messages from producer with priority.

// Producer program

#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

using namespace std;

int main() {
  // creating message queue
  mq_open("/mq_157", O_CREAT | O_RDWR, 0644, NULL);
  int fd = mq_open("/mq_157", O_WRONLY);

  if (fd == -1) {
    perror("mq_open");
    return 1;
  }

  while (true) {
    // handle EOF to break loop
    string message;
    cout << "Enter message to send (or EOF to exit): ";
    if (!getline(cin, message)) {
      // sending consumer "exit" message to terminate consumer
      mq_send(fd, "exit", 4, 0);
      break;
    }

    cout << "Enter priority for the message (0-31): ";
    unsigned int priority;
    cin >> priority;
    cin.ignore();  // Ignore the newline character left by cin

    // Validate priority range
    if (priority > 31) {
      cout << "Invalid priority. Please enter a value between 0 and 31." << endl;
      continue;
    }

    mq_send(fd, message.c_str(), message.length(), priority);
  }

  // close and unlink message queue
  mq_close(fd);
  mq_unlink("/mq_157");

  exit(0);
}