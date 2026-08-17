/* Write a program which handles SIGINT (signal) - when arrived once - just show
interupt requested when arrived twice - exit the program */

#include <signal.h>
#include <unistd.h>

#include <iostream>

void sigint_handler(int signum) {
  static int count = 0;
  if (count == 0) {
    std::cout << "Interrupt requested. Press Ctrl+C again to exit." << std::endl;
  } else if (count == 1) {
    std::cout << "Exiting program." << std::endl;
    exit(0);
  }
  count++;
}

int main() {
  std::cout << "Process started. PID: " << getpid() << std::endl;

  // Register signal handler for SIGINT
  struct sigaction sa{};
  sa.sa_flags = 0;
  sa.sa_handler = sigint_handler;   // function pointer to the handler
  sigaction(SIGINT, &sa, nullptr);  // Register the signal handler in Process's signal table

  int i = 0;
  while (true) {
    // Infinite loop to keep the process running
    sleep(1);
    std::cout << i << " - I am running..." << std::endl;
    i++;
  }
}