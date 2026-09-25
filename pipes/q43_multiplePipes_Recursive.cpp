// Write program which gonna run like > ./prog and then user inputs "command1" "command2" "command3" ... and redirect the output of command1 to command2 and output of command2 to command3 and so on, like a pipe chain.

// User required to enter command in format: ./prog "command1" "command2" "command3" .., (double quotes)
#include <unistd.h>

#include <iostream>
#include <sstream>

using namespace std;

vector<char*> formatCommand(const std::string& str);
void recFxn(int index, const std::vector<std::string>& commands);

int main() {
  string input;
  cout << "Enter command in format: \"command1\" \"command2\" \"command3\" .. :  ";
  getline(cin, input);

  // tokenize arguments into string commands to string array
  // parse arguments character by character
  bool inQuotes = false;
  std::vector<std::string> commands;
  std::string currentCommand;

  for (int i = 0; i < input.length(); i++) {
    char c = input[i];
    if (c == '"') {
      inQuotes = !inQuotes;  // Toggle inQuotes state
      if (!inQuotes && !currentCommand.empty()) {
        commands.push_back(currentCommand);
        currentCommand.clear();
      }
    } else if (inQuotes) {
      currentCommand += c;
    }
  }
  if (inQuotes) {
    cerr << "Error: Mismatched quotes in input." << endl;
    return 1;
  }

  // printing commands array to debugg
  cout << "Commands to execute: " << endl;
  for (const auto& cmd : commands) {
    cout << cmd << endl;
  }

  recFxn(0, commands);  // Start the recursive function with the first command
}

void recFxn(int index, const std::vector<std::string>& commands) {
  if (index >= commands.size()) {
    return;  // Base case: no more commands to execute
  } else if (index == commands.size() - 1) {
    // Last command: execute it and redirect output to stdout
    std::vector<char*> args = formatCommand(commands[index]);
    execvp(args[0], args.data());
    perror("execvp failed");
    exit(EXIT_FAILURE);
  } else {
    int fd[2];
    if (pipe(fd) == -1) {
      perror("pipe failed");
      exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
      perror("fork failed");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      // Child process: redirect input from pipe and execute next command
      close(fd[1]);               // Close write end of the pipe
      dup2(fd[0], STDIN_FILENO);  // Redirect stdin to read end of the pipe
      close(fd[0]);               // Close the original read end of the pipe

      recFxn(index + 1, commands);
    } else {
      // Parent process: redirect output to pipe and execute current command
      close(fd[0]);                // Close read end of the pipe
      dup2(fd[1], STDOUT_FILENO);  // Redirect stdout to write end of the pipe
      close(fd[1]);                // Close the original write end of the pipe

      std::vector<char*> args = formatCommand(commands[index]);
      execvp(args[0], args.data());
      perror("execvp failed");
      exit(EXIT_FAILURE);
    }
  }
}

// Helper function: converts string into a NULL-terminated array of char*
std::vector<char*> formatCommand(const std::string& str) {
  std::stringstream ss(str);
  std::string token;
  std::vector<char*> args;

  while (ss >> token) {
    // strdup allocates heap memory for the token copy
    args.push_back(strdup(token.c_str()));
  }
  args.push_back(nullptr);  // Required by execvp

  return args;
}