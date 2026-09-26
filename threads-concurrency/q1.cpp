// Write program that creates thread (worker thread) and they prints message "hey, I am worker thread" and main thread prints "hey, I am main thread" and then main thread waits for worker thread to finish and then terminates.

#include <unistd.h>

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

void workerThreadFunction() {
  cout << "hey, I am worker thread" << endl;
  return;
}

int main() {
  // Create a worker thread
  thread workerThread(workerThreadFunction);

  // Main thread prints its message
  cout << "hey, I am main thread" << endl;

  // Wait for the worker thread to finish
  workerThread.join();

  return 0;
}
