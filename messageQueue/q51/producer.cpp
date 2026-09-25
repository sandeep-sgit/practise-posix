/* q51. Create program that takes n number from user. then pass it to producer and create n workers

// producer gonna take n as argument and creates 2n jobs and sends to pipe and then terminates dont sends "STOP"

// worker simply takes jobs and prints and waiting for "STOP" to stop

these is experiment to demenstrate if producer fails/crash - observe the workers and their life. */

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

  // Send 2n jobs
  for (int i = 1; i <= 2 * n; ++i) {
    string job = "Job-" + to_string(i);
    strncpy(message.mtext, job.c_str(), sizeof(message.mtext));
    msgsnd(msgid, &message, sizeof(message.mtext), 0);
    cout << "Producer Sent: " << job << endl;
  }

  // Producer terminates without sending "STOP"

  // Do NOT delete queue here; workers are still reading!
  return 0;
}