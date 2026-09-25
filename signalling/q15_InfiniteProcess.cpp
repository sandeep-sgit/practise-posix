// Write program to be killed by SIGKILL signal

#include <iostream>
#include <unistd.h>

int main() {
  std::cout << "Process started. PID: " << getpid() << std::endl;
  int i = 0;
  while (true) {
    // Infinite loop to keep the process running
    sleep(1);
    std::cout << i << " - I am running..." << std::endl;
    i++;
  }
}
