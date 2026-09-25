// Create an deliberate deadlock using 2 shared resources and 2 mutexes and 2 threads

#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

// global shared resources
int resource1 = 0;
int resource2 = 0;

// global mutexes
mutex mtx1;
mutex mtx2;

void thread1() {
  // Lock the first mutex
  mtx1.lock();
  cout << "Thread 1 locked resource 1" << endl;

  // Simulate some work
  this_thread::sleep_for(chrono::milliseconds(100));

  // Now try to lock the second mutex
  cout << "Thread 1 trying to lock resource 2" << endl;
  mtx2.lock();
  cout << "Thread 1 locked resource 2" << endl;

  // Unlock the mutexes
  mtx2.unlock();
  mtx1.unlock();
}

void thread2() {
  // Lock the second mutex
  mtx2.lock();
  cout << "Thread 2 locked resource 2" << endl;

  // Simulate some work
  this_thread::sleep_for(chrono::milliseconds(100));

  // Now try to lock the first mutex
  cout << "Thread 2 trying to lock resource 1" << endl;
  mtx1.lock();
  cout << "Thread 2 locked resource 1" << endl;

  // Unlock the mutexes
  mtx1.unlock();
  mtx2.unlock();
}

int main() {
  thread t1(thread1);
  thread t2(thread2);

  t1.join();
  t2.join();

  return 0;
}
