// Write program which gonna run like > ./prog and then user inputs "command1" "command2" "command3" ... and redirect the output of command1 to command2 and output of command2 to command3 and so on, like a pipe chain.

// User required to enter command in format: ./prog "command1" "command2" "command3" .., (double quotes)

/* here Parent should normally be pipeline manager, creating all processes and manging lifecycle */

#include <unistd.h>

#include <iostream>
#include <sstream>

using namespace std;

vector<char*> formatCommand(const std::string& str);

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

  // Creating array of pipes
  int numCommands = commands.size();
  int pipes[numCommands - 1][2];

  for (int i = 0; i < numCommands - 1; i++) {
    if (pipe(pipes[i]) == -1) {
      perror("pipe");
      exit(EXIT_FAILURE);
    }
  }

  // Spawn child processes
  for (int i = 0; i < numCommands; i++) {
    pid_t pid = fork();

    if (pid == -1) {
      perror("fork failed");
      return 1;
    }

    if (pid == 0) {  // Child Process
      // Redirect STDIN from previous pipe if not the first command
      if (i > 0) {
        dup2(pipes[i - 1][0], STDIN_FILENO);
      }

      // Redirect STDOUT to next pipe if not the last command
      if (i < numCommands - 1) {
        dup2(pipes[i][1], STDOUT_FILENO);
      }

      // Close ALL pipe descriptors in the child so EOF can propagate
      for (int j = 0; j < numCommands - 1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }

      // Execute command
      auto args = formatCommand(commands[i]);
      execvp(args[0], args.data());

      perror("execvp failed");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < numCommands - 1; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  // Wait for all child processes to finish
  for (int i = 0; i < numCommands; i++) {
    wait(nullptr);
  }

  // Exit the program
  return 0;
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