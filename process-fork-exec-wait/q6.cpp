/* Write program in which parent creates (user given) child process, and parents
 waits for all 
 Two possibilities- 
 1st - execution gonna like worker 1, then worker 2 then worker 3 ... gonna execute one by one 
 2nd - execution gonna like worker 1, worker 2, worker 3, started parallelly, and parent gonna wait
 for all child to finish, and then parent gonna print "all child are done
*/

//2nd : Parallel Workers

#include <iostream>
#include <unistd.h>

int main() {
  int n;
  std::cout << "Enter number of child processes to create: ";
  std::cin >> n;

  for (int i = 0; i < n; i++) {
    int pid = fork();

    if (pid == 0) { // Child Process
      std::cout << "Child process " << i + 1 << " started. Child Number = " << i + 1 << std::endl;
      sleep(2); // Simulate work by sleeping for 2 seconds
      std::cout << "Child process " << i + 1 << " finished." << std::endl;
      exit(0);               // Exit child process
    }
  }

  // Parent Process waits for all children to finish
  for (int i = 0; i < n; i++) {
    wait(NULL);
  }

  std::cout << "All child processes are done." << std::endl;
}