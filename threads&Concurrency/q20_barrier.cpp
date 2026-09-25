/* We gona understand barrier in cpp, by an example
This program is simulating an image processor- we simple take image divide them in 4 parts 4 threads (for its 4 portions) then
wait upto an point wher eall threads have done processing then we simulate compressoin */

#include <barrier>  //C++20 standard barrier header
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono_literals;

int image = 4;                   // Simulated image divided into 4 parts
std::barrier sync_point(image);  // Barrier for synchronizing threads

void process_image_part(int time) {
  cout << "Thread is processing part of the image." << endl;
  this_thread::sleep_for(chrono::seconds(time));  // FIXED: Converts integer 'time' into seconds

  // Wait for all threads to reach this point
  sync_point.arrive_and_wait();

  cout << "Thread has finished processing and is now compressing the image." << endl;
  this_thread::sleep_for(5s);  // Simulate compression time
}

int main() {
  vector<thread> threads;

  // Create threads to process image parts
  for (int i = 0; i < image; ++i) {
    threads.emplace_back(process_image_part, i * 5);  // Simulate different processing times for each part
  }

  // Join threads
  for (auto& t : threads) {
    t.join();
  }

  cout << "All threads have finished processing and compressing the image." << endl;
  return 0;
}