// Write program that prints something --> then creates new process (child
// process-fork) --> then again print something

#include <iostream>
#include <unistd.h>

int main() {

  std::cout << "This is the parent process." << std::endl;

  fork(); // Creating a new process

  std::cout << "This executed after creating Another process (child)"
            << std::endl;
}