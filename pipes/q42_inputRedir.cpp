/* Write code to input redirection ex program starts and take command < /file and redirect input to command that's it*/

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::string input;
  std::cout << "Enter command in format: command < input_file - ";
  std::getline(std::cin, input);

  // tokenize the input string into arguments
  std::vector<std::string> args;
  std::string arg;
  for (char c : input) {
    if (c == ' ') {
      if (!arg.empty()) {
        args.push_back(arg);
        arg.clear();
      }
    } else {
      arg += c;
    }
  }
  if (!arg.empty()) {  // push last argument
    args.push_back(arg);
  }

  // Validate args and split
  std::string command = "";
  std::string input_file = "";
  bool foundRedir = false;

  for (size_t i = 0; i < args.size(); i++) {
    if (args[i] == "<") {
      foundRedir = true;
      continue;
    } else if (!foundRedir) {
      command += args[i] + " ";
    } else {
      input_file += args[i] + " ";
    }
  }

  // FIX: Remove the trailing spaces added by the loop above
  if (!command.empty()) command.pop_back();
  if (!input_file.empty()) input_file.pop_back();

  if (!foundRedir) {
    std::cerr << "Error: No redirection operator '<' found." << std::endl;
    return 1;
  }

  // print current directory of process for debugging
  std::cout << "Current working directory: " << getcwd(nullptr, 0) << std::endl;

  int fd = open(input_file.c_str(), O_RDONLY);
  if (fd == -1) {
    std::cerr << "Error: Could not open input file." << std::endl;
    return 1;
  }

  // Redirect stdin to the input file
  dup2(fd, STDIN_FILENO);
  close(fd);

  // Execute the command
  execlp(command.c_str(), command.c_str(), (char*)NULL);
  std::cerr << "Error: Could not execute command." << std::endl;
  return 1;
}