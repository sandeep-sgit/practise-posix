/* Create program which takes arguments (in which command 1 > command 2 : kind of format is there)
   parent executes command 1 (and stdout connected to pipe write end)
   child executes command 2 (stdin is connected with pipe read end)*/

   // Actually its better to say it is question of pipe '|' , '>' is redirection operator, '|' is pipe operator, but in this question we are gonna use '>' as pipe operator.

#include <unistd.h>

#include <iostream>

int main() {
  std::cout << "Enter command in format: command1 > command2" << std::endl;
  std::string input;
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
  if (!arg.empty()) {
    args.push_back(arg);
  }

  // Validate args and split command 1 and command 2 by '>', if no redir-opertator found then exit with error message
  if (args.size() < 3) {
    std::cerr << "Usage: " << input << " command1 > command2 " << std::endl;
    return 1;
  }

  // make two strings for command1 and command2 and split the arguments into two strings
  std::string command1 = "";
  std::string command2 = "";
  bool foundRedir = false;
  for (int i = 0; i < args.size(); i++) {
    if (std::string(args[i]) == ">") {
      foundRedir = true;
      continue;
    } else if (!foundRedir) {
      command1 += args[i];
      command1 += " ";
    } else {
      command2 += args[i];
      command2 += " ";
    }
  }

  if (!foundRedir) {
    std::cerr << "Error: No redirection operator '>' found." << std::endl;
    return 1;
  }

  // Create a pipe
  int fd[2];
  if (pipe(fd) == -1) {
    std::cerr << "Pipe creation failed" << std::endl;
    return 1;
  }

  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "Fork failed" << std::endl;
    return 1;
  }

  if (pid == 0) {               // Child process
    close(fd[1]);               // Close unused write end
    dup2(fd[0], STDIN_FILENO);  // Redirect stdin to read end of the pipe
    close(fd[0]);               // Close the original read end

    // Execute command2
    execlp("/bin/sh", "sh", "-c", command2.c_str(), nullptr);
    std::cerr << "Execution of command2 failed" << std::endl;
    exit(1);
  } else {                       // Parent process
    close(fd[0]);                // Close unused read end
    dup2(fd[1], STDOUT_FILENO);  // Redirect stdout to write end of the pipe
    close(fd[1]);                // Close the original write end

    // Execute command1
    execlp("/bin/sh", "sh", "-c", command1.c_str(), nullptr);
    std::cerr << "Execution of command1 failed" << std::endl;
    exit(1);
  }

  wait(NULL);  // Wait for child process to finish
  return 0;
}

/* GEMINI PROPER CODE

#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>

int main() {
  std::cout << "Enter command in format: command1 > command2" << std::endl;
  std::string input;
  std::getline(std::cin, input);

  // Tokenize the input string into arguments
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
  // Push the very last argument if the string didn't end with a space
  if (!arg.empty()) {
      args.push_back(arg);
  }

  // Validate args: minimum 3 arguments (cmd1, >, cmd2)
  if (args.size() < 3) {
    std::cerr << "Usage: command1 > command2" << std::endl;
    return 1;
  }

  // Split the arguments into command1 and command2
  std::string command1 = "";
  std::string command2 = "";
  bool foundRedir = false;

  // Start at i = 0, not i = 1
  for (size_t i = 0; i < args.size(); i++) {
    if (args[i] == ">") {
      foundRedir = true;
      continue;
    } else if (!foundRedir) {
      command1 += args[i] + " ";
    } else {
      command2 += args[i] + " ";
    }
  }

  if (!foundRedir) {
    std::cerr << "Error: No redirection operator '>' found." << std::endl;
    return 1;
  }

  // Create a pipe
  int fd[2];
  if (pipe(fd) == -1) {
    std::cerr << "Pipe creation failed" << std::endl;
    return 1;
  }

  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "Fork failed" << std::endl;
    return 1;
  }

  if (pid == 0) {               // Child process
    close(fd[1]);               // Close unused write end
    dup2(fd[0], STDIN_FILENO);  // Redirect stdin to read end of the pipe
    close(fd[0]);               // Close the original read end

    // Execute command2
    execlp("/bin/sh", "sh", "-c", command2.c_str(), nullptr);
    std::cerr << "Execution of command2 failed" << std::endl;
    exit(1);
  } else {                       // Parent process
    close(fd[0]);                // Close unused read end
    dup2(fd[1], STDOUT_FILENO);  // Redirect stdout to write end of the pipe
    close(fd[1]);                // Close the original write end

    // Execute command1
    execlp("/bin/sh", "sh", "-c", command1.c_str(), nullptr);
    std::cerr << "Execution of command1 failed" << std::endl;

    // Note: If execlp succeeds, the parent process is replaced.
    // Anything below this line will only run if execlp fails.
    exit(1);
  }

  return 0;
} */