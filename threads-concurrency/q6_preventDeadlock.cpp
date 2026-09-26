// prevent deadlock by ordering. 2 shared resources, 2 mutees, 2 threads.  Each thread locks the resources in the same order, preventing deadlock.

/* Global Ordering for all locks (shared resource) is only solution for preventing deadlock
   Here A < B (priority order)*/

#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

// Shared resources
int resourceA = 0;
int resourceB = 0;

// Corresponding mutexes for the shared resources
mutex mutexA;
mutex mutexB;

void threadFunction1() {
  // Lock resources in a specific order to prevent deadlock
  mutexA.lock();
  cout << "Thread 1 locked resource A" << endl;

  mutexB.lock();
  cout << "Thread 1 locked resource B" << endl;

  // Simulate some work
  cout << "Thread 1 finished work" << endl;

  // Unlock the mutexes
  mutexB.unlock();
  mutexA.unlock();
}

void threadFunction2() {
  // Lock resources in the same order to prevent deadlock
  mutexA.lock();
  cout << "Thread 2 locked resource A" << endl;

  mutexB.lock();
  cout << "Thread 2 locked resource B" << endl;

  // Simulate some work
  cout << "Thread 2 finished work" << endl;

  // Unlock the mutexes
  mutexB.unlock();
  mutexA.unlock();
}

int main() {
  thread t1(threadFunction1);
  thread t2(threadFunction2);

  t1.join();
  t2.join();

  return 0;
}

// In these manner we can prevent deadlock by just ordering the mutex locks for shared resources.  In this case, both threads lock resource A first and then resource B, preventing the circular wait condition that leads to deadlock.