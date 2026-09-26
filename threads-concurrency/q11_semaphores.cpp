/* Write an program in which user gives N (no or request/worker)
   and each worker simply doing pring : worker i started then worker i exiting (waits 1 sec inside)
   We gonna limit the no of request/worker at a time upto 3 */

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <semaphore> // C++20 standard semaphore header

using namespace std;
using namespace std::chrono;

// Initialize counting semaphore with 3 max permits
std::counting_semaphore<3> sem(3);

void worker(int i) {
    sem.acquire(); // Decrements permit count (replaces sem_wait)

    cout << "Worker " << i << " started" << endl;
    this_thread::sleep_for(seconds(1));
    cout << "Worker " << i << " exiting" << endl;

    sem.release(); // Increments permit count (replaces sem_post)
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

    // Destruction is automatic — no sem_destroy required!
    return 0;
}