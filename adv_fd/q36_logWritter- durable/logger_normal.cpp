/* In previous question - we wrote logger experiment code and we here going for test its durable and normal version
logger- durable - is with fsync() call
logger- normal is without fsync() call*/

#include <fcntl.h>
#include <unistd.h>

#include <csignal>
#include <fstream>
#include <iostream>
#include <string>

namespace {
volatile std::sig_atomic_t signal_received = 0;

void handle_sigint(int) { signal_received = 1; }
}  // namespace

int main(int argc, char* argv[]) {
  // 1. Validate command line arguments
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <logfile>" << std::endl;
    return 1;
  }

  // 2. Open the log file for appending
  int log_fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (log_fd == -1) {
    std::cerr << "Error opening log file: " << argv[1] << std::endl;
    return 1;
  }

  // 3. Set up signal handler for SIGINT (Ctrl+C)
  std::signal(SIGINT, handle_sigint);

  // 4. Main loop to read input and append to log file
  while (!signal_received) {
    // std::cout << "Enter message : ";
    std::string message;
    std::getline(std::cin, message);

    if (std::cin.eof()) {  // handles Ctrl+D : EOF Marker to program
      break;               // Exit loop on Ctrl+D
    }

    // Append message to log file
    int n = write(log_fd, message.c_str(), message.length());
    if (n == -1) {
      std::cerr << "Error writing to log file" << std::endl;
      return 1;
    } else if (n < static_cast<int>(message.length())) {
      std::cerr << "Partial write to log file" << std::endl;
      return 1;
    }
    n = write(log_fd, "\n", 1);
    if (n == -1) {
      std::cerr << "Error writing newline to log file" << std::endl;
      return 1;
    }
  }

  // Close the log file
  close(log_fd);
  std::cout << "\nExiting logger. Goodbye!" << std::endl;
  return 0;
}
