// Create Zombie Process, & observe in terminal

#include <iostream>
#include <unistd.h>

int main(){
    int pid = fork();

    if (pid == 0) { // Child Process
        std::cout << "Child process exiting..." << std::endl;
        exit(42); // Child exits immediately
    } else if (pid > 0) { // Parent Process
        std::cout << "Parent : I gonna work for 45 seconds" << std::endl;

        for (int i = 0; i < 45; ++i) {
            sleep(1);
            std ::cout << "Parent working: " << i + 1 << " seconds" << std::endl;
        }
        std::cout << "Parent process exiting... & NOT COLLECTEDCHILD" << std::endl;
    }
}