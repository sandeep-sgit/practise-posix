/* Write program to handle SIGINT in such a way, SIGNAL --> Process --> Cleanup --> Exit */

#include <signal.h>
#include <unistd.h>

#include <csignal>
#include <iostream>

volatile std::sig_atomic_t cleanup_flag = 0;

void handle_sigint(int signum) {
  std::cout << "Received SIGINT signal. Cleaning up before exiting..." << std::endl;
  // Perform any necessary cleanup here
  // For example, closing files, releasing resources, etc.
  cleanup_flag = 1;  // Set the flag to exit the loop in main
}

int main() {
  std::cout << "Process started. PID: " << getpid() << std::endl;

  signal(SIGINT, handle_sigint);  // Register signal handler for SIGINT

  int i = 0;
  while (cleanup_flag == 0) {
    // Infinite loop to keep the process running
    sleep(1);
    std::cout << i << " - I am running..." << std::endl;
    i++;
  }

  std::cout << "Cleanup completed. Exiting program." << std::endl;
  exit(0);
}