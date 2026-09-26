/* Write an program with only 1 worker at a time (1 semaphore)
    Take N (no of request/worker) as input from the user
    Each worker will print a message when it starts and exits, with a 1-second delay in between.

    Observe : why its different from mutes*/

#include <chrono>
#include <iostream>
#include <semaphore>  // C++20 standard semaphore header
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

// Initialize binary semaphore with 1 max permit
std::binary_semaphore sem(1);

mutex mtx;  // Mutex for comparison

void worker1(int i) {
  sem.acquire();  // Decrements permit count (replaces sem_wait)

  cout << "Worker " << i << " started" << endl;
  this_thread::sleep_for(seconds(1));
  cout << "Worker " << i << " exiting" << endl;

  sem.release();  // Increments permit count (replaces sem_post)
}

void worker2(int i) {
  lock_guard<mutex> lock(mtx);  // Lock the mutex

  cout << "Worker " << i << " started" << endl;
  this_thread::sleep_for(seconds(1));
  cout << "Worker " << i << " exiting" << endl;

  // Mutex is automatically released when lock_guard goes out of scope
}

int main() {
  int N;
  cout << "Enter the number of workers: ";
  cin >> N;

  vector<thread> workers;
  workers.reserve(N);

  for (int i = 0; i < N; ++i) {
    workers.emplace_back(worker1, i);
  }

  for (auto& worker : workers) {
    worker.join();
  }

  cout << "All workers have completed." << endl;

  // Now let's try the mutex version
  cout << "\nNow running with mutex for comparison:\n";
  workers.clear();  // Clear the previous vector of threads
  for (int i = 0; i < N; ++i) {
    workers.emplace_back(worker2, i);
  }

  for (auto& worker : workers) {
    worker.join();
  }

  // Destruction is automatic — no sem_destroy required!
  return 0;
}