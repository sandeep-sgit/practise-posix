// Write program that runs infinitely and handle the sigint and sigterm signals
// in process

#include <iostream>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int signum) {
  std::cout << "Received SIGINT signal. FuckOFF SIGINT - I am not exiting"
            << std::endl;
}

void handle_sigterm(int signum) {
  std::cout << "Received SIGTERM signal. FuckOFF SIGTERM - I am not exiting"
            << std::endl;
}

int main() {
  std::cout << "Process started. PID: " << getpid() << std::endl;

  // Register signal handlers
  struct sigaction sa_int{};
  sa_int.sa_flags = 0;
  sa_int.sa_handler = handle_sigint;
  sigaction(SIGINT, &sa_int, nullptr);

  struct sigaction sa_term{};
  sa_term.sa_flags = 0;
  sa_term.sa_handler = handle_sigterm;
  sigaction(SIGTERM, &sa_term, nullptr);

  int i = 0;
  while (true) {
    // Infinite loop to keep the process running
    sleep(1);
    std::cout << i << " - I am running..." << std::endl;
    i++;
  }
}