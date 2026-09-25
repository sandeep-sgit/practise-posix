/* Write an program for rendezvous between two threads (Semaphore use - synchronization pattern)
   We gonna spin two threads each doing to wait upto user given number of seconds.
   when that work happens we use rendezvous point in both thread- to synchronize their execution.
   then print Thread-X continuing */

#include <chrono>
#include <iostream>
#include <semaphore>  //C++20 standard semaphore header
#include <thread>

using namespace std;
using namespace std::chrono;

std::counting_semaphore<1> a_arrived(0);  // Semaphore for thread 1
std::counting_semaphore<1> b_arrived(0);  // Semaphore for thread 2

void threadA(int wait_time) {
  cout << "Thread A waiting for " << wait_time << " seconds." << endl;
  this_thread::sleep_for(seconds(wait_time));
  cout << "Thread A has finished waiting." << endl;

  a_arrived.release();  // Signal that thread A has arrived at the rendezvous point
  b_arrived.acquire();  // Wait for thread B to arrive at the rendezvous point

  cout << "Thread A continuing after rendezvous." << endl;
}

void threadB(int wait_time) {
  cout << "Thread B waiting for " << wait_time << " seconds." << endl;
  this_thread::sleep_for(seconds(wait_time));
  cout << "Thread B has finished waiting." << endl;

  b_arrived.release();  // Signal that thread B has arrived at the rendezvous point
  a_arrived.acquire();  // Wait for thread A to arrive at the rendezvous point

  cout << "Thread B continuing after rendezvous." << endl;
}

int main() {
  int wait_time_A, wait_time_B;
  cout << "Enter wait time for Thread A (in seconds): ";
  cin >> wait_time_A;
  cout << "Enter wait time for Thread B (in seconds): ";
  cin >> wait_time_B;

  thread tA(threadA, wait_time_A);
  thread tB(threadB, wait_time_B);

  tA.join();
  tB.join();

  cout << "Both threads have completed their execution." << endl;

  return 0;
}
