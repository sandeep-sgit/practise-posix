/* Write an simple program to demonstrate the use of available variables in C */

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
#include <chrono>

using namespace std::chrono;

int sharedVariable = 0;
bool available = false;
mutex mtx;
condition_variable cv;

void producerThread() {
  unique_lock<mutex> lock(mtx);
  {
    sharedVariable = 10;  // Produce a value
    cout << "Enter value for Producer to produced: ";
    cin >> sharedVariable;

    available = true;  // Set the available to true
  }
  lock.unlock();

  cv.notify_one();  // Notify one waiting thread

  return;
}

void workerThread() {
  unique_lock<mutex> lock(mtx);
  {
    cv.wait(lock, [] { return available; });  // Wait until available is true
    cout << "Worker consumed: " << sharedVariable << endl;
  }
  lock.unlock();

  return;
}

int main() {
  thread producer(producerThread);
  thread worker(workerThread);

  // Simulate some work in main thread
  this_thread::sleep_for(chrono::seconds(1));

  producer.join();
  worker.join();

  return 0;
}
