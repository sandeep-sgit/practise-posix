/* We gonna creat an program in which producer sends data to consumer using queue of size 3
Last time I used Condition Variable, these time I have to use Semaphores */

#include <iostream>
#include <queue>
#include <semaphore>  // C++20 standard semaphore header
#include <thread>

using namespace std;

queue<int> buffer;   // Shared buffer
mutex buffer_mutex;  // Mutex to protect access to the buffer

std::counting_semaphore<3> empty_slots(3);  // Semaphore to track empty
std::counting_semaphore<3> full_slots(0);   // Semaphore to track full slots

void producer(int items_to_produce) {
  for (int i = 0; i < items_to_produce; ++i) {
    empty_slots.acquire();  // Wait for an empty slot
    {
      lock_guard<mutex> lock(buffer_mutex);
      buffer.push(i);
      cout << "Producer produced item " << i << endl;
    }
    full_slots.release();  // Signal that a new item is available
  }
}

void consumer() {
  while (true) {
    full_slots.acquire();  // Wait for a full slot
    int item;
    {
      lock_guard<mutex> lock(buffer_mutex);
    
      item = buffer.front();
      buffer.pop();
      cout << "Consumer consumed item " << item << endl;
    }
    empty_slots.release();  // Signal that an empty slot is available
  }
}

int main() {
  int items_to_produce;
  cout << "Enter number of items to produce: ";
  cin >> items_to_produce;

  thread prod_thread(producer, items_to_produce);
  thread cons_thread(consumer);

  prod_thread.join();
  cons_thread.detach();  // Detach consumer thread since it runs indefinitely

  return 0;
}
