// Write an program in which producer sends message to consumer with priority.
// Consumer gonna receive message here instantly so start consumer process after sending messages from producer with priority.

// Consumer program

#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

using namespace std;

int main() {
  // creating message queue- not necessary for consumer
  // mq_open("/mq_157", O_CREAT | O_RDWR, 0644, NULL);

  // REPLACED: POSIX functions return mqd_t (message queue descriptor type), not int
  // int fd = mq_open("/mq_157", O_RDONLY);
  mqd_t fd = mq_open("/mq_157", O_RDONLY);

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
    // REPLACED: Fixed 8192 static array replaced by dynamic buffer allocated using mq_getattr()
    // char buffer[8192];
    // ssize_t bytesRead = mq_receive(fd, buffer, sizeof(buffer), NULL);

    // consume message in priority order- highest priority message gonna be received first
    ssize_t bytesRead = mq_receive(fd, buffer, attr.mq_msgsize, NULL);

    if (bytesRead == -1) {
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