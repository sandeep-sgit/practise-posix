/* Write an v_A and v_B for demonstrating thread->detach and jthread.
   Here an simple example is take our progrma starts and it spins logger thread which simply loops (and takes log) and write it to log file
   For stimulating use 100 log lines (100 time for loop and simply write the line number in log file and each write takes 10 ms (stimulate these also))
   For main thread- keep it to run for 500 ms (stimulating the work)

   In reality log thread is connected to queue for (taking log of all threads) but here we just stimulating
   and in reality it runs while (true) here we just taking example of 100 log received for understanding JTHREAD */

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

// The background logger worker
void logger_task() {
  std::ofstream log_file("log.txt");

  for (int i = 1; i <= 100; ++i) { // stimulate 100 log lines
    log_file << "log line " << i << "\n";
    log_file.flush();  // Force the write to disk instantly

    // Sleep for 10ms to simulate the time it takes to process and write
    std::this_thread::sleep_for(10ms);
  }
}

int main() {
  std::cout << "[Main] Starting logger thread...\n";

  // ==========================================
  // VERSION A: Detach (Abandon the thread)
  // ==========================================
//   std::thread t(logger_task);
//   t.detach();

  // ==========================================
  // VERSION B: jthread (Own the thread)
  // To test Version B, comment out Version A above,
  // and uncomment the line below:
  // ==========================================
   std::jthread t(logger_task);

  std::cout << "[Main] Simulating server work for 200ms...\n";
  std::this_thread::sleep_for(200ms);

  std::cout << "[Main] Work done. Exiting program NOW.\n";
  return 0;  // The process is destroyed here
}