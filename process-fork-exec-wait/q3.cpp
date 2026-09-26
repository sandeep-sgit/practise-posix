// Write program in which an variable declared and after both child & parent
// updates its value --> see what is Result

#include <iostream>
#include <unistd.h>

int main() {
  int x = 100;

  std::cout << "Initial value of x: " << x << std::endl;

  fork(); // Creating a new process

  x += 10; // Both parent and child will update the value of x

  std::cout << "Value of x after update: " << x << std::endl;
}