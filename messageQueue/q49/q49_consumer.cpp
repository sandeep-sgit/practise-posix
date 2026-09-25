// Write program to demonstrate non blocking message queu- consumer side & handle EAGIN error

// Consumer program

#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

using namespace std;

int main() {
  // creating message queue- not necessary for consumer
  // int fd = mq_open("/mq_157", O_RDONLY);
  mqd_t fd = mq_open("/mq_157", O_RDONLY | O_NONBLOCK);  // Open in non-blocking mode

  // Handling case - when message queue is not exists.
  // REPLACED: Compare against (mqd_t)-1 instead of bare integer -1 for standard compliance
  // if (fd == -1) {
  if (fd == (mqd_t)-1) {
    perror("mq_open");
    return 1;
  }

  // STANDARD PRACTICE: Dynamically fetch queue attributes to guarantee the buffer size matches mq_msgsize
  struct mq_attr attr;
  mq_getattr(fd, &attr);
  char* buffer = new char[attr.mq_msgsize + 1];

  while (true) {
    ssize_t bytesRead = mq_receive(fd, buffer, attr.mq_msgsize, NULL);

    // Handle EAGAIN error for non-blocking read
    if (bytesRead == -1) {
      if (errno == EAGAIN) {
        cout << "No messages available in the queue." << endl;
        continue;
      }
      perror("mq_receive");
      delete[] buffer;  // STANDARD PRACTICE: Free dynamic memory before exiting on error
      return 1;
    }

    // Handling case - when producer sends "exit" message to terminate consumer
    if (strncmp(buffer, "exit", 4) == 0) {
      cout << "Consumer received exit message. Terminating..." << endl;
      break;
    }

    buffer[bytesRead] = '\0';  // Null-terminate the string
    cout << "Consumer received: " << buffer << endl;
  }

  // STANDARD PRACTICE: Clean up dynamically allocated memory
  delete[] buffer;

  // close and unlink message queue
  mq_close(fd);
  // mq_unlink("/mq_157"); // Not necessary for consumer, only producer should

  exit(0);
}