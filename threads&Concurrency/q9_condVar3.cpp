/* Write an program which takes x and y (no of producers and no of consumers)
   Each producer produces number from 10*p (p is pth producer) to 10*p + 10
   Each consumer consumes the numbers produced by the producers

   Learning : Use Mutex + 2 Condition variable + Thread
              Queue size is limited to 3
              */

// In previous example we used one cv, but here we used 2 cv for better performance, one for producer and one for consumer. So that when producer is waiting, it will not notify consumer and vice versa.

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;
using namespace std::chrono;

queue<int> q;
mutex mtx;                       // Here mutex is for Whole queue
condition_variable cv_producer;  // condition variable for producer
condition_variable cv_consumer;  // condition variable for consumer

void producerThread(int p) {
  for (int j = 10 * p; j < 10 * p + 10; ++j) {
    unique_lock<mutex> lock(mtx);
    cv_producer.wait(lock, [] { return q.size() < 3; });  // Wait if queue is full

    q.push(j);  // Produce a value

    lock.unlock();
    cv_consumer.notify_one();  // Notify one waiting consumer
  }

  return;
}

void consumerThread(int c) {
  while (true) {
    unique_lock<mutex> lock(mtx);
    cv_consumer.wait(lock, [] { return !q.empty(); });  // Wait if queue is empty

    int value = q.front();
    q.pop();  // Consume a value
    cout << "Consumer " << c << " consumed: " << value << endl;

    lock.unlock();
    cv_producer.notify_one();  // Notify one waiting producer
  }

  return;
}

int main() {
  int x, y;
  cout << "Enter the number of producers: ";
  cin >> x;
  cout << "Enter the number of consumers: ";
  cin >> y;

  vector<thread> producers;
  vector<thread> consumers;

  for (int i = 0; i < x; ++i) {
    producers.emplace_back(producerThread, i);
  }

  for (int i = 0; i < y; ++i) {
    consumers.emplace_back(consumerThread, i);
  }

  for (auto& producer : producers) {
    producer.join();
  }

//   for (auto& consumer : consumers) {
//     consumer.join();
//   }

  // I am forcing the consumers to shut down (i dont know terminate process), thus I simply exit program here
  exit(0);  // It forces consumer thread to shutdown
}
