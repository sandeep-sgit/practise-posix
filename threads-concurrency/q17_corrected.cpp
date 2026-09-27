// write bounder producer-consumer program using queue (and semaphores)

#include <iostream>
#include <queue>
#include <semaphore>  // C++20 standard semaphore header
#include <thread>

using namespace std;

queue<int> buffer;            // Shared buffer
bool productionDone = false;  // Flag to indicate if production is done
mutex mtx;                    // Mutex for both buffer and productionDone flag

std::counting_semaphore<3> empty_slots(3);   // Semaphore to track empty slots
std::counting_semaphore<> full_slots(0);  // Semaphore to track

void producer(int i) {
  for (int j = i * 10; j < i * 10 + 10; ++j) {
    empty_slots.acquire();  // Wait for an empty slot
    {
      lock_guard<mutex> lock(mtx);
      buffer.push(j);
      cout << "Producer " << i << " produced item " << j << endl;
    }
    full_slots.release();  // Signal that a new item is available
  }
}

void consumer(int i) {
  while (true) {
    full_slots.acquire();  // Wait for a full slot

    {  // covers permit released from main thread (when production is done)
      lock_guard<mutex> lock(mtx);
      if (buffer.empty() && productionDone) {
        break;  // Exit if production is done and buffer is empty
      }
      int item = buffer.front();
      buffer.pop();
      cout << "Consumer " << i << " consumed item " << item << endl;
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

  for (int i = 0; i < num_producers; ++i) {
    producers.emplace_back(producer, i);
  }

  for (int i = 0; i < num_consumers; ++i) {
    consumers.emplace_back(consumer, i);
  }

  for (auto& t : producers) {
    t.join();
  }

  {  // set productionDone flag
    lock_guard<mutex> lock(mtx);
    productionDone = true;
  }

  for (int i = 0; i < num_consumers; ++i) {
    full_slots.release();  // Release the semaphore to unblock consumers
  }

  for (auto& t : consumers) {
    t.join();
  }

  return 0;
}