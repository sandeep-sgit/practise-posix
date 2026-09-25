/* Write a program in which threads gonna use MUTEX to synchronize access to a shared resource.
   We gonna create program which has i = 0 globally and it spins 2 thread each thread tries to fxn i - 1 million times
   and see the final output */

#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

int i = 0;  // Shared resource

mutex mtx;  // Mutex for synchronization

void fxn() {
  for (int j = 0; j < 1000000; ++j) {
    mtx.lock();  // Lock the mutex before accessing the shared resource
    i++;
    mtx.unlock();  // Unlock the mutex after accessing the shared resource
  }
}

int main() {
  thread t1(fxn);
  thread t2(fxn);

  t1.join();
  t2.join();

  cout << "Final value of i: " << i << endl;  // Should be 2 million if synchronized correctly

  return 0;
}
