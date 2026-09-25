/* Write an program which takes x and y (no of producers and no of consumers)
   Each producer produces number from 10*p (p is pth producer) to 10*p + 10
   Each consumer consumes the numbers produced by the producers

   Learning : Graceful Shutdown of worker thread using condition variable and mutex
              */

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

bool productionDone = false;  // Flag to indicate if production is done

void producerThread(int p) {
  for (int j = 10 * p; j < 10 * p + 10; ++j) {
    unique_lock<mutex> lock(mtx);
    cv_producer.wait(lock, [] { return q.size() < 3; });  // Wait if queue is full

    q.push(j);  // Produce a value

    lock.unlock();
    cv_consumer.notify_one();  // Notify one waiting consumer
  }

  // productionDone = true;     // Set the production done flag --- !!!Wrong set for all production done!! in main code
  // cv_consumer.notify_all();  // Notify all waiting consumers

  return;
}

void consumerThread(int c) {
  while (true) {
    unique_lock<mutex> lock(mtx);
    cv_consumer.wait(lock, [] { return !q.empty() || productionDone; });  // Wait if queue is empty

    if (productionDone && q.empty()) {
      break;
    }

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

  mtx.lock();
  productionDone = true;  // Set the production done flag after all producers are done
  mtx.unlock();
  cv_consumer.notify_all();  // Notify all waiting consumers

  for (auto& consumer : consumers) {
    consumer.join();
  }
}
