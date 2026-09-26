//Write program to see orphan child- Observe the transition of PPID from parent to init process in child (via Terminal also)

#include <iostream>
#include <unistd.h>

int main(){
    int pid = fork();

    if (pid == 0) { // Child Process
        std::cout << "Child process: My PID is " << getpid() << ", My PPID is " << getppid() << std::endl;

        for (int i = 0; i < 30; ++i) {
            sleep(1);
            std::cout << "Child working: " << i + 1 << " seconds, My PPID is " << getppid() << std::endl;
        }

        std::cout << "Parent is Exited now. Let's check my Reparenting..." << std::endl;

        std::cout << "Child process: My PID is " << getpid() << ", My PPID is " << getppid() << std::endl;

        std::cout << "I am waiting for 30 seconds... , You can check on Terminal htop & ps -fp for parent" << std::endl;
        sleep(30);
    } else if (pid > 0) { // Parent Process
        std::cout << "Parent : I gonna work for 30 seconds" << std::endl;
        sleep(30);
        std::cout << "Parent process exiting..." << std::endl;
    }
}