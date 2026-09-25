#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

// --- SHARED STATE ---
int stock_price = 100;        // Realistic shared resource
int active_readers = 0;       // Tracks readers currently looking at data
bool is_writing = false;      // True if a writer is updating data

mutex mtx;                    // Protects state variables (active_readers & is_writing)
condition_variable cv;        // Coordinates sleeping and waking threads

void reader(int id, int total_reads) {
    for (int i = 0; i < total_reads; ++i) {
        // 1. ENTRY: Wait for writers to finish, then increment reader count
        {
            unique_lock<mutex> lock(mtx);
            // SAFE WHEN: No writer is currently modifying data
            cv.wait(lock, [] { return !is_writing; });
            active_readers++;
        } // Lock is released here so other readers can enter concurrently!

        // 2. READ: Multiple readers execute this TOGETHER with no lock
        cout << "[Reader " << id << "] Current Stock Price: $" << stock_price << endl;
        this_thread::sleep_for(100ms); // Simulate reading time

        // 3. EXIT: Decrement reader count and alert waiting writer if last reader
        {
            unique_lock<mutex> lock(mtx);
            active_readers--;
            if (active_readers == 0) {
                cv.notify_all(); // Last reader out alerts waiting writers
            }
        }

        this_thread::sleep_for(150ms); // Pause before next read iteration
    }
}

void writer(int id, int total_writes) {
    for (int i = 0; i < total_writes; ++i) {
        // 1. ENTRY: Wait until NO writer is active AND NO readers are active
        {
            unique_lock<mutex> lock(mtx);
            // SAFE WHEN: No writing in progress AND zero readers actively reading
            cv.wait(lock, [] { return !is_writing && active_readers == 0; });
            is_writing = true; // Lock out incoming readers and writers
        }

        // 2. WRITE: Exclusive update step
        stock_price += 15; // Realistic update
        cout << "  ===> [Writer " << id << "] UPDATED Stock Price to: $" << stock_price << endl;
        this_thread::sleep_for(200ms); // Simulate write duration

        // 3. EXIT: Clear write flag and wake everyone waiting
        {
            unique_lock<mutex> lock(mtx);
            is_writing = false;
            cv.notify_all(); // Wake up all waiting readers & writers
        }

        this_thread::sleep_for(300ms); // Pause before next write iteration
    }
}

int main() {
    int num_readers, num_writers;
    cout << "Enter number of readers: ";
    cin >> num_readers;
    cout << "Enter number of writers: ";
    cin >> num_writers;

    // Use std::vector for standard, safe thread management
    vector<thread> readers;
    vector<thread> writers;

    // Spin readers (each will read 3 times)
    for (int i = 0; i < num_readers; ++i) {
        readers.emplace_back(reader, i + 1, 3);
    }

    // Spin writers (each will write 2 times)
    for (int i = 0; i < num_writers; ++i) {
        writers.emplace_back(writer, i + 1, 2);
    }

    // Clean execution finish (no infinite loops)
    for (auto& t : readers) t.join();
    for (auto& t : writers) t.join();

    cout << "\nExecution finished cleanly! Final Stock Price: $" << stock_price << endl;
    return 0;
}