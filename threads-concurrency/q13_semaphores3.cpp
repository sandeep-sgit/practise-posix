/* Design a multi-threaded program that takes an integer N as input from the user, spawns N worker threads, and restricts simultaneous execution to a maximum of 3 active workers using a counting semaphore. The program must track and display the live count of active workers in real time throughout
 * its entire execution journey.*/

#include <chrono>
#include <iostream>
#include <semaphore>  // C++20 standard semaphore header
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

mutex mtx;               // Mutex for synchronizing access to active worker count
int active_workers = 0;  // Counter for active workers

std::counting_semaphore<3> sem(3);  // Counting semaphore with a maximum of 3 permits

void worker(int i) {
  sem.acquire();  // Decrements permit count (replaces sem_wait)

  {
    lock_guard<mutex> lock(mtx);  // Lock the mutex to update active worker count
    active_workers++;
    cout << "Worker " << i << " started. Active workers: " << active_workers << endl;
  }

  this_thread::sleep_for(seconds(1));  // Simulate work with a 1-second delay

  {
    lock_guard<mutex> lock(mtx);  // Lock the mutex to update active worker count

    active_workers--;
  }

  sem.release();  // Increments permit count (replaces sem_post)
}

int main() {
  int N;
  cout << "Enter the number of workers: ";
  cin >> N;

  vector<thread> workers;
  workers.reserve(N);

  for (int i = 0; i < N; ++i) {
    workers.emplace_back(worker, i);
  }

  for (auto& worker : workers) {
    worker.join();
  }

  cout << "All workers have completed." << endl;

  return 0;
}
