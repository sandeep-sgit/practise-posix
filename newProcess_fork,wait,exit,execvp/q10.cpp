/* Write code in which process forks child and child gonna run for 5 second, at same time
   Parent doing its work 1sec interval and checks wheather child exited or not, whien child exits parent 
   Parent print the exit code of child and breaks*/

#include <iostream>
#include <unistd.h>

int main(){
    int pid = fork();

    if (pid == 0) { // Child Process
        for (int i = 0; i < 5; ++i) {
            std::cout << "Child working..." << std::endl;
            sleep(1);
        }
        return 42; // Child exits with code 42
    } else if (pid > 0) { // Parent Process
        while (true) {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG); // Non-blocking wait
            if (result == 0) {
                std::cout << "Parent working..." << std::endl;
                sleep(1);
            } else if (result == pid) {
                if (WIFEXITED(status)) {
                    std::cout << "Child exited with code: " << WEXITSTATUS(status) << std::endl;
                }
                break; // Exit the loop when child has exited
            } else {
                std::cerr << "Error waiting for child process." << std::endl;
                break;
            }
        }
    }
}