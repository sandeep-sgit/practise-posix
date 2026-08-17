/* Write program to handle SIGKILL & observe*/

#include <signal.h>
#include <unistd.h>

#include <iostream>

void handle_sigkill(int signum) {
  std::cout << "Received SIGKILL signal. I cannot handle SIGKILL - I will be terminated." << std::endl;
  std::cout << "I am trying to not exit here.. I will tryyyyy.." << std::endl;
}

int main() {
  std::cout << "Process started. PID: " << getpid() << std::endl;

  signal(SIGKILL, handle_sigkill);  // Attempt to register signal handler for SIGKILL
  signal(SIGTERM, handle_sigkill);  // Register signal handler for SIGTERM
  signal(SIGINT, handle_sigkill);   // Register signal handler for SIGINT

  int i = 0;
  while (true) {
    // Infinite loop to keep the process running
    sleep(1);
    std::cout << i << " - I am running..." << std::endl;
    i++;
  }
}