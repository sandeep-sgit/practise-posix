/* Write an program where producer produces numbers from 0 to i (i given by user) and he puts in queu (of limited size 3 blocks)
   and another hand consumer consuming the queu (and whhen its empty consumer should wait) and if its fulll producer should wait
*/

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;
using namespace std::chrono;

queue<int> q;
mutex mtx;              // Here mutex is for Whole queue
condition_variable cv;  // one condition variable for both producer and consumer

void producerThread() {
  int i;
  cout << "Enter the value of i (upper limit for production): ";
  cin >> i;

  for (int j = 0; j <= i; ++j) {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return q.size() < 3; });  // Wait if queue is full

    q.push(j);  // Produce a value

    lock.unlock();
    cv.notify_one();  // Notify one waiting consumer
  }

  // add -1 to indicate end of production
  unique_lock<mutex> lock(mtx);
  cv.wait(lock, [] { return q.size() < 3; });  // Wait if queue is full
  q.push(-1);                                  // Indicate end of productiond
  lock.unlock();
  cv.notify_one();  // Notify one waiting consumer

  return;
}

void consumerThread() {
  while (true) {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return !q.empty(); });  // Wait if queue is empty

    int value = q.front();
    q.pop();  // Consume a value

    lock.unlock();
    cv.notify_one();  // Notify one waiting producer

    if (value == -1) {
      break;  // Exit if end of production is indicated
    }

    cout << "Consumer consumed: " << value << endl;
  }
  return;
}

int main() {
  thread producer(producerThread);
  thread consumer(consumerThread);

  producer.join();
  consumer.join();

  return 0;
}
