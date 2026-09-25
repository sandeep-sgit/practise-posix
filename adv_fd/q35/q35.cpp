/* I gonna create an experiment in which :

I gonna create an program which simply runs and forks child and both gonna take input from stdin
and after fork ---> I gonna write the input to stdout for both the child as well as parent
So I want to see how the behaviour gonna will behaviour
I mean - case1 : both child and parent gets input same and prints both twice
case2 : child and parent can fight for input and can take partial input and while printing from particulars i will gonna know that
which one consumed what? */

// Observe : does child gets same fd form parent process???

#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>

int main() {
  int pid = fork();
  if (pid == -1) {
    std::cerr << "Fork failed" << std::endl;
    return 1;
  } else if (pid == 0) {  // Child process
    std::string input;
    // taking input from stdin
    std::cout << "Child process: Enter input: ";
    std::getline(std::cin, input);
    std::cout << "Child process received: " << input << std::endl;
  } else {  // Parent process
    std::string input;
    // taking input from stdin
    std::cout << "Parent process: Enter input: ";
    std::getline(std::cin, input);
    std::cout << "Parent process received: " << input << std::endl;
  }
  // I DONT KNOW WHAT PARENT CAN PRINT AND CHILD - INTERESTING 
  return 0;
}