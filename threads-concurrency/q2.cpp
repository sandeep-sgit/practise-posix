/* I gonna create multiple threads program- simply take number from user and create that much thread and each thread will print thread-x started and then thread-x finished- I want simply observe the order of their execution*/

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void threadFunction(int threadId) {
  cout << "Thread-" << threadId << " started" << endl;
  // Simulate some work with sleep
  this_thread::sleep_for(chrono::milliseconds(100));
  cout << "Thread-" << threadId << " finished" << endl;
}

int main() {
  int numThreads;
  cout << "Enter the number of threads to create: ";
  cin >> numThreads;

  vector<thread> threads;

  for (int i = 0; i < numThreads; ++i) {
    threads.emplace_back(threadFunction, i + 1);
  }

  // join all thread
  for (auto& t : threads) {
    t.join();
  }

  return 0;
}
