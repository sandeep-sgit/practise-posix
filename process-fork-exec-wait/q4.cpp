// Write code in which child gonna work for 5 seconds & parents gonna wait, and
// in end parent collects child exit code Parent prints "child is done" & parent
// prints child exit code

#include <iostream>
#include <unistd.h>

int main() {
  int pid = fork();

  if (pid == 0) { // Child Process
    std::cout << "Child process started, waiting for 5 seconds..." << std::endl;
    for (int i = 0; i < 5; ++i) {
      sleep(1);
      std::cout << "Child waiting: " << i + 1 << " seconds" << std::endl;
    }
  } else if (pid > 0) { // Parent Process
    int status;
    waitpid(pid, &status, 0); // Wait for child to finish
    std::cout << "Child is done" << std::endl;
    std::cout << "Child exit code: " << WEXITSTATUS(status) << std::endl;
  }
}