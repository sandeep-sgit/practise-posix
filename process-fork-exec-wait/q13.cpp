// Write program that takes string as input from user and hands it over like
// (command typed in terminal) to run new recipe.

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

struct CommandArgs {
  std::vector<std::string> tokens;
  std::vector<char *> argv;
};

CommandArgs parseCommand(const std::string &cmd) {
  CommandArgs result;

  std::istringstream iss(cmd);
  std::string word;

  while (iss >> word)
    result.tokens.push_back(word);

  for (auto &s : result.tokens)
    result.argv.push_back(const_cast<char *>(s.c_str()));

  result.argv.push_back(nullptr);

  return result;
}

int main() {
  std::string command;

  std::cout << "Enter a command to execute: ";
  std::getline(std::cin, command);

  CommandArgs args = parseCommand(command);

  // Replace current process
  execvp(args.argv[0], args.argv.data());

  // gonna execute - if execvp fails, print error message
  std::cerr << "Error executing command: " << std::endl;
}
