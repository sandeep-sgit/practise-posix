/* Use shared_mutex for our previous question q18_ReaderWriter.cpp */
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono_literals;

shared_mutex mtx;       // Shared mutex (Requires C++17)
int stock_price = 100;  // Shared resource

void reader(int id, int total_reads) {
  for (int i = 0; i < total_reads; ++i) {
    // Inner block creates a scope so lock automatically releases BEFORE sleeping
    {
      shared_lock<shared_mutex> lock(mtx); // Acquire Shared Lock
      cout << "[Reader " << id << "] Current Stock Price: $" << stock_price << endl;
    } // Lock released HERE!

    this_thread::sleep_for(100ms); // Simulate reading delay WITHOUT holding the lock
  }
}

void writer(int id, int total_writes) {
  for (int i = 0; i < total_writes; ++i) {
    // Inner block creates a scope so lock automatically releases BEFORE sleeping
    {
      unique_lock<shared_mutex> lock(mtx); // Acquire Exclusive Lock
      stock_price += 15;
      cout << "  ===> [Writer " << id << "] UPDATED Stock Price to: $" << stock_price << endl;
    } // Lock released HERE!

    this_thread::sleep_for(200ms); // Simulate writing delay WITHOUT holding the lock
  }
}

int main() {
  int num_readers, num_writers;
  cout << "Enter number of readers: ";
  cin >> num_readers;
  cout << "Enter number of writers: ";
  cin >> num_writers;

  vector<thread> readers;
  vector<thread> writers;

  for (int i = 0; i < num_readers; ++i) {
    readers.emplace_back(reader, i + 1, 5);
  }

  for (int i = 0; i < num_writers; ++i) {
    writers.emplace_back(writer, i + 1, 3);
  }

  for (auto& r : readers) r.join();
  for (auto& w : writers) w.join();

  return 0;
}