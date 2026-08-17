// Write program that loops and takes command from user and run it in new
// process. (like a shell)

#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct CommandArgs {
  std::vector<std::string> tokens;
  std::vector<char*> argv;
};

CommandArgs takeInput();                           // Function to take input from user
CommandArgs parseCommand(const std::string& cmd);  // Function to parse command into tokens
int handleBuiltin(CommandArgs args);               // Function to handle built-in commands like 'cd'
void handleCommand(CommandArgs args);              // Function to handle external commands
void printArgv(const std::vector<char*>& argv);    // Function to print the command arguments for debugging

int main() {
  while (true) {  // Shell loop

    CommandArgs args = takeInput();  // Take input from user

    int builtinResult = handleBuiltin(args);  // Handle built-in commands like 'cd' and 'exit' | It returns 0 - > No Builtin, 1 - > Builtin handled, 2 - > exit command
    if (builtinResult == 1) {
      continue;  // Built-in command handled, continue to next iteration
    } else if (builtinResult == 2) {
      break;  // Exit the shell
    }

    handleCommand(args);  // Handle external commands
  }
}

// helps to take input from user and parse it into tokens (args)
CommandArgs takeInput() {
  std::string command;
  // Prompt user for command
  std::cout << "Enter a command to execute (or 'exit' to quit): ";
  std::getline(std::cin, command);
  CommandArgs args = parseCommand(command);  // Parse it
  return args;
}

CommandArgs parseCommand(const std::string& cmd) {
  CommandArgs result;

  std::istringstream iss(cmd);
  std::string word;

  while (iss >> word) result.tokens.push_back(word);

  for (auto& s : result.tokens) result.argv.push_back(const_cast<char*>(s.c_str()));

  result.argv.push_back(nullptr);

  return result;
}

// Function to Handle Built-in commands like 'cd' and 'exit'
int handleBuiltin(CommandArgs args) {
  // 1. Check if user pressed Enter without typing any command
  if (args.tokens.empty() || args.argv[0] == nullptr) {
    return 1;  // Handle silently and prompt again
  }

  // 2. Safely compare strings using std::strcmp or std::string
  std::string command = args.tokens[0];

  if (command == "exit") {
    return 2;  // Signal exit command
  }

  if (command == "cd") {
    if (args.argv[1] == nullptr) {
      std::cerr << "Expected argument to \"cd\"\n";
    } else {
      int result = chdir(args.argv[1]);

      if (result == 0) {
        std::cout << "Changed directory to: " << args.argv[1] << std::endl;
        std::cout << getcwd(nullptr, 0) << std::endl;
      } else {
        std::perror("cd failed reee");
      }
    }
    return 1;  // Signal that a built-in command was handled
  }

  return 0;  // Signal that no built-in command was handled
}

// Function to Handle External Commands
void handleCommand(CommandArgs args) {
  // Fork a new process to execute the command
  int pid = fork();

  if (pid == 0) {                            // Child Process
    execvp(args.argv[0], args.argv.data());  // Replace current process

    std::cerr << "Error executing command: " << std::endl;  // If execvp fails, print error message
    exit(EXIT_FAILURE);
  } else if (pid > 0) {  // Parent Process
    int status;
    waitpid(pid, &status, 0);  // Wait for the child process to finish
  } else {
    std::cerr << "Fork failed!" << std::endl;
  }
}

// Function to print the command arguments for debugging
void printArgv(const std::vector<char*>& argv) {
  std::cout << "[ ";
  for (const char* arg : argv) {
    if (arg == nullptr) {
      std::cout << "NULL";
    } else {
      std::cout << "\"" << arg << "\" ";
    }
  }
  std::cout << "]\n";
}