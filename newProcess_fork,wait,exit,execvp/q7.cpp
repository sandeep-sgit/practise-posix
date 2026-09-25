// Write code to which forks child and child returns "0" exit code and parent collects it and prints

#include <iostream>
#include <unistd.h>

int main(){
    int pid = fork();

    if (pid == 0) { // Child Process
        return 0;
    } else if (pid > 0) { // Parent Process
        int status;
        wait(&status); // Wait for child to finish
        if (WIFEXITED(status)) {
            std::cout << "Child exited with code: " << WEXITSTATUS(status) << std::endl;
        }
    } 
}