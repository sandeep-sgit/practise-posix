// Write program which takes PID and signal no. as argument to these executable, and gonna send signal to that process

#include <unistd.h>

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  // Check if the correct number of arguments is provided
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <PID> <Signal Number>\n";
    return EXIT_FAILURE;
  }

  // Convert arguments to appropriate types
  pid_t pid = static_cast<pid_t>(std::stoi(argv[1]));
  int signal_number = std::stoi(argv[2]);

  // Send the signal to the specified process
  if (kill(pid, signal_number) == -1) {
    perror("Error sending signal");
    return EXIT_FAILURE;
  }

  std::cout << "Signal " << signal_number << " sent to process " << pid << std::endl;
  return EXIT_SUCCESS;
}