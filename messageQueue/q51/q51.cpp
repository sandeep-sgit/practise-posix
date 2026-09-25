/* q51. Create program that takes n number from user. then pass it to producer and create n workers

// producer gonna take n as argument and creates 2n jobs and sends to pipe and then terminates dont sends "STOP"

// worker simply takes jobs and prints and waiting for "STOP" to stop

these is experiment to demenstrate if producer fails/crash - observe the workers and their life. */

#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

int main() {
  // Ensure the backing file exists for ftok
  int fd = open("messageQueue", O_CREAT | O_RDWR, 0666);
  if (fd != -1) close(fd);

  int n;
  cout << "Enter the number of workers: ";
  cin >> n;

  // Fork n workers
  for (int i = 0; i < n; i++) {
    if (fork() == 0) {
      execlp("./worker", "./worker", nullptr);
      exit(EXIT_FAILURE);
    }
  }

  // Fork producer
  if (fork() == 0) {
    execlp("./producer", "./producer", to_string(n).c_str(), nullptr);
    exit(EXIT_FAILURE);
  }

  // Wait for producer + n workers to exit
  for (int i = 0; i < n + 1; i++) {
    wait(nullptr);
  }

  // Clean up queue system-wide after everyone finishes
  key_t key = ftok("messageQueue", 65);
  int msgid = msgget(key, 0666);
  if (msgid != -1) {
    msgctl(msgid, IPC_RMID, nullptr);
  }

 
  return 0;
}