/* Write Minishell and introduce redirection '>' */

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct CommandArgs {
  std::vector<std::string> tokens;
  std::vector<char*> argv;
};

CommandArgs takeInput();                                                               // Function to take input from user
CommandArgs parseCommand(const std::string& cmd);                                      // Function to parse command into tokens
int handleBuiltin(CommandArgs args);                                                   // Function to handle built-in commands like 'cd'
void handleExternalCommand(CommandArgs args);                                          // Function to handle external commands
void printArgv(const std::vector<char*>& argv);                                        // Function to print the command arguments for debugging
void handleBackgroundCommand(CommandArgs args);                                        // Function to handle background commands
void sigchild_handler(int signum);                                                     // Signal handler for SIGCHLD to reap background processes
void handleRedirectionCommand(CommandArgs args);                                       // Function to handle redirection commands
void handleCommand(CommandArgs args);                                                  // Function to handle commands (built-in, external, background, redirection)
void parseCommand(CommandArgs& args);                                                  // Function to parse tokens into argv for execvp
int getTokenIndex(const std::vector<std::string>& tokens, const std::string& target);  // Function to return the index of a token if found, or -1 if not

int main() {
  signal(SIGCHLD, sigchild_handler);  // Register the signal handler for SIGCHLD

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

void handleCommand(CommandArgs args) {
  if (getTokenIndex(args.tokens, ">") != -1) {
    handleRedirectionCommand(args);  // Handle redirection commands
  } else if (args.tokens.back() == "&") {
    handleBackgroundCommand(args);  // Handle background commands
  } else {
    handleExternalCommand(args);  // Handle external commands
  }
}

// Function to return the index if found, or -1 if not present.
int getTokenIndex(const std::vector<std::string>& tokens, const std::string& target) {
  auto it = std::find(tokens.begin(), tokens.end(), target);

  if (it != tokens.end()) {
    return std::distance(tokens.begin(), it);  // Returns the actual index
  }

  return -1;  // Not found
}

// helps to take input from user and parse it into tokens (args)
CommandArgs takeInput() {
  std::string command;
  // Prompt user for command
  std::cout << "> Minishell- $  ";
  std::getline(std::cin, command);
  CommandArgs args = parseCommand(command);  // Parse it
  return args;
}

CommandArgs parseCommand(const std::string& cmd) {  // Function to parse String command into tokens
  CommandArgs result;

  std::istringstream iss(cmd);
  std::string word;

  while (iss >> word) result.tokens.push_back(word);

  for (auto& s : result.tokens) result.argv.push_back(const_cast<char*>(s.c_str()));

  result.argv.push_back(nullptr);

  return result;
}

void parseCommand(CommandArgs& args) {  // Function to parse tokens into argv for execvp
  args.argv.clear();                    // Clear existing argv to avoid duplicates
  for (auto& s : args.tokens) {
    args.argv.push_back(const_cast<char*>(s.c_str()));
  }
  args.argv.push_back(nullptr);
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
void handleExternalCommand(CommandArgs args) {
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

// Function to handle background commands
void handleBackgroundCommand(CommandArgs args) {
  args.tokens.pop_back();  // Remove '&' from tokens

  args.argv.pop_back();          // Remove 'nullptr' from argv
  args.argv.pop_back();          // Remove '&' from argv
  args.argv.push_back(nullptr);  // Add 'nullptr' back to argv

  int pid = fork();
  if (pid == 0) {                            // Child process
    execvp(args.argv[0], args.argv.data());  // Replace current process
    std::cerr << "Error executing command: " << std::endl;
    exit(EXIT_FAILURE);
  }
  // Parent process continues here
}

void handleRedirectionCommand(CommandArgs args) {
  // Handle redirection
  int redirectIndex = getTokenIndex(args.tokens, ">");

  // 1. Validate that exactly one token follows '>'
  // If the index isn't exactly the second-to-last item, the path is invalid.
  if (redirectIndex != args.tokens.size() - 2) {
    std::cerr << "Error: enter valid path (exactly one file expected after >)\n";
    return;  // Skip execution and return to shell prompt
  }

  // 2. Store the path and strip the redirection from the command
  std::string path = args.tokens.back();  // Gets the last element
  args.tokens.erase(args.tokens.begin() + redirectIndex, args.tokens.end());
  parseCommand(args);  // Re-parse to update argv after removing redirection tokens

  // 3. Path validity: If it's a directory, append the default file name
  if (fs::exists(path) && fs::is_directory(path)) {
    // Standard shells throw an "Is a directory" error here,
    // but as per your logic, we append a default file.
    path += "/file.txt";
  }

  // 4. Fork and execute
  pid_t pid = fork();
  if (pid == 0) {
    // CHILD PROCESS

    // Open file: Write only, create if missing, truncate if exists. Permissions: rw-r--r--
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
      perror("Error opening file");
      exit(1);
    }

    // Redirect Standard Output (fd 1) to our file descriptor
    dup2(fd, STDOUT_FILENO);
    close(fd);  // Close the original fd since stdout now points to the file

    execvp(args.argv[0], args.argv.data());  // Execute the command
    perror("Error executing command");
    exit(1);
  } else if (pid > 0) {  // PARENT PROCESS
    int status;
    waitpid(pid, &status, 0);  // Wait for the child process to
  } else {
    perror("Fork failed");
  }
}

// Signal handler for SIGCHLD to reap background processes
void sigchild_handler(int signum) {
  int status;
  int pid = waitpid(-1, &status, WNOHANG);  // Reap any terminated child process without blocking
  while (pid > 0) {                         // Reap all terminated child processes
    pid = waitpid(-1, &status, WNOHANG);    // Check for more terminated child processes
  }
}
