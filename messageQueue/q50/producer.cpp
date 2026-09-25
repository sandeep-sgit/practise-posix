#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

struct msg_buffer {
  long mtype;
  char mtext[256];
};

int main(int argc, char* argv[]) {
  if (argc != 2) return 1;
  int n = atoi(argv[1]);

  key_t key = ftok("messageQueue", 65);
  int msgid = msgget(key, 0666 | IPC_CREAT);

  msg_buffer message;
  message.mtype = 1;

  // Send 3n + 1 jobs
  for (int i = 1; i <= 3 * n + 1; ++i) {
    string job = "Job-" + to_string(i);
    strncpy(message.mtext, job.c_str(), sizeof(message.mtext));
    msgsnd(msgid, &message, sizeof(message.mtext), 0);
    cout << "Producer Sent: " << job << endl;
  }

  // Send n STOP messages
  for (int i = 0; i < n; ++i) {
    string stopMsg = "STOP";
    strncpy(message.mtext, stopMsg.c_str(), sizeof(message.mtext));
    msgsnd(msgid, &message, sizeof(message.mtext), 0);
    cout << "Producer Sent: STOP" << endl;
  }

  // Do NOT delete queue here; workers are still reading!
  return 0;
}