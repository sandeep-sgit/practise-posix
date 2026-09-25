#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

struct msg_buffer {
  long mtype;
  char mtext[256];
};

int main() {
  key_t key = ftok("messageQueue", 65);
  int msgid = msgget(key, 0666 | IPC_CREAT);

  msg_buffer message;

  while (true) {
    // msgrcv with mtype 0 reads the next available message in order
    if (msgrcv(msgid, &message, sizeof(message.mtext), 0, 0) == -1) {
      break;
    }

    string job(message.mtext);
    if (job == "STOP") {
      cout << "Worker [" << getpid() << "] received STOP. Exiting." << endl;
      break;
    }

    cout << "Worker [" << getpid() << "] received: " << job << endl;
  }

  return 0;
}