/* Here are multiple producers and multiple consumers take the numbers from user
   Producer produces ith to i*10 + 10 numbers and consumers gonna simply consume all
   use semaphroes in it */

#include <iostream>
#include <queue>
#include <semaphore>  // C++20 standard semaphore header
#include <thread>

using namespace std;
using namespace std::chrono_literals;

queue<int> buffer;   // Shared buffer
mutex buffer_mutex;  // Mutex to protect access to the buffer

std::counting_semaphore<3> empty_slots(3);  // Semaphore to track empty slots
std::counting_semaphore<3> full_slots(0);   // Semaphore to track full slots

void producer(int id) {
  for (int i = id * 10; i < id * 10 + 10; ++i) {
    empty_slots.acquire();  // Wait for an empty slot
    {
      lock_guard<mutex> lock(buffer_mutex);
      buffer.push(i);
      cout << "Producer " << id << " produced item " << i << endl;
    }
    full_slots.release();  // Signal that a new item is available
  }
}

void consumer(int id) {
  while (true) {
    full_slots.acquire();  // Wait for a full slot
    int item;
    {
      lock_guard<mutex> lock(buffer_mutex);
      item = buffer.front();
      buffer.pop();
      cout << "Consumer " << id << " consumed item " << item << endl;
    }
    empty_slots.release();  // Signal that an empty slot is available
  }
}

int main() {
  int num_producers, num_consumers;
  cout << "Enter number of producers: ";
  cin >> num_producers;
  cout << "Enter number of consumers: ";
  cin >> num_consumers;

  vector<thread> producers;
  vector<thread> consumers;

  // Create producer threads
  for (int i = 0; i < num_producers; ++i) {
    producers.emplace_back(producer, i);
  }

  // Create consumer threads
  for (int i = 0; i < num_consumers; ++i) {
    consumers.emplace_back(consumer, i);
  }

  // Wait for all producer threads to complete
  for (auto& t : producers) {
    t.join();
  }

  // Wait for all consumer threads to complete
  for (auto& t : consumers) {
    t.join();
  }

  return 0;
}