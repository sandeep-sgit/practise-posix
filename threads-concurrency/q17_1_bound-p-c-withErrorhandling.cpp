/* Producer gonna generate images (number given by user) now image falls in 3 categories (valid, known error, unknown error)

    Producer gonna pick random number from N/2 for known error and from N/4 for unknown error, leftones are valid.
    Now after we have categories (3 category for image) we simply gonna push image in queue using counter draw method and Label them like "image_x : <category>"
    So producer pushes all images randomly (using counter draw method)

    on another hand Consumer gonna consume and use if statement (for know error) and for unknown error we gonna catch it (we here explicitly create unknown error class)
    Also Consumer gonna push jobResut for each image in vector<jobResult> and jobResult is kind of struct (with string image, bool ok, string error // empty if ok )
    for consumers - to process image use 50 ms wait to stimulate

    and in end we gonna read the processedResults and check how much valid and how much know error and how much unkwon error.

    Here we have to use semaphore for synchronization. */

#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <semaphore>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono_literals;

struct jobResult {
  string image;
  bool ok;
  string error;
};

// create a custom exception class for unknown errors
class UnknownError : public std::runtime_error {
 public:
  UnknownError(const std::string& message) : std::runtime_error(message) {}
};

queue<string> imageQueue;     // Shared buffer for images
bool productionDone = false;  // Flag to indicate if production is done
mutex mtx;                    // Mutex to protect access to the buffer

vector<jobResult> processedResults;  // Vector to store processed results

std::counting_semaphore<3> empty_slots(3);  // Semaphore to track empty slots
std::counting_semaphore<> full_slots(0);    // Semaphore to track full slots

void producer(int numImages) {
  // CHANGE 1: Simplified and cleaned up variable names for clarity
  int totalLeft = numImages;

  random_device rd;
  mt19937 gen(rd());

  // CHANGE 2: Consolidated the random generation for cleaner initialization
  int numKnown = uniform_int_distribution<>(0, numImages / 3)(gen);
  int numUnknown = uniform_int_distribution<>(0, numImages / 3)(gen);
  int numValid = numImages - numKnown - numUnknown;

  // CHANGE 3: Added a print statement so you can verify if 0 unknown errors were rolled
  cout << "\nStarting Counts -> Valid: " << numValid << ", Known: " << numKnown << ", Unknown: " << numUnknown << "\n\n";

  for (int i = 1; i <= numImages; ++i) {
    empty_slots.acquire();

    // CHANGE 4: Using uniform_int_distribution instead of modulo (%) for better, unbiased randomness
    uniform_int_distribution<> dist(0, totalLeft - 1);
    int roll = dist(gen);

    string category;

    // The counter draw boundaries updated to match the new clean variable names
    if (roll < numValid) {
      category = "valid";
      numValid--;
    } else if (roll < numValid + numKnown) {
      category = "known error";
      numKnown--;
    } else {
      category = "unknown error";
      numUnknown--;
    }

    {
      lock_guard<mutex> lock(mtx);
      string image = "image_" + to_string(i) + " : " + category;
      imageQueue.push(image);
    }
    totalLeft--;
    full_slots.release();
  }
}

void consumer(int id) {
  while (true) {
    full_slots.acquire();  // Wait for a full slot
    string image;
    {
      lock_guard<mutex> lock(mtx);

      // CHANGE 5: Added a robust safety check. If the queue is empty, it shouldn't pop.
      // If production is done, it breaks. If not, it skips this loop iteration.
      if (imageQueue.empty()) {
        if (productionDone) break;
        continue;
      }

      image = imageQueue.front();
      imageQueue.pop();
    }

    // Simulate processing time
    this_thread::sleep_for(50ms);

    jobResult result;
    result.image = image;

    // Process the image and handle errors
    try {
      // CHANGE 7: Swapped the order of if-statements.
      // "unknown error" MUST be checked before "known error" to prevent string matching overlaps.
      if (image.find("unknown error") != string::npos) {
        throw UnknownError("Unknown error occurred");
      } else if (image.find("known error") != string::npos) {
        result.ok = false;
        result.error = "Known error occurred";
      } else if (image.find("valid") != string::npos) {
        result.ok = true;
        result.error = "";
      }
    } catch (const UnknownError& e) {
      result.ok = false;
      result.error = "Unknown error occurred";
    }

    {
      lock_guard<mutex> lock(mtx);
      processedResults.push_back(result);
    }

    empty_slots.release();
  }
}

int main() {
  int numImages;
  cout << "Enter number of images to produce: ";
  cin >> numImages;

  cout << "Enter number of consumers: ";
  int numConsumers;
  cin >> numConsumers;

  // spinning 1 producer thread and multiple consumer threads
  thread producerThread(producer, numImages);
  vector<thread> consumers;
  for (int i = 0; i < numConsumers; ++i) {
    consumers.emplace_back(consumer, i);
  }

  producerThread.join();
  {
    lock_guard<mutex> lock(mtx);
    productionDone = true;
  }

  // CHANGE 6: Replaced the for-loop with a single, cleaner semaphore release call
  full_slots.release(numConsumers);

  for (auto& t : consumers) {
    t.join();
  }

  // Analyze processed results
  int validCount = 0;
  int knownErrorCount = 0;
  int unknownErrorCount = 0;
  for (const auto& result : processedResults) {
    if (result.ok) {
      validCount++;
    } else if (result.error == "Known error occurred") {
      knownErrorCount++;
    } else if (result.error == "Unknown error occurred") {
      unknownErrorCount++;
    }
  }

  // display the results
  cout << "Processed Results:\n";
  cout << "Valid: " << validCount << "\n";
  cout << "Known Errors: " << knownErrorCount << "\n";
  cout << "Unknown Errors: " << unknownErrorCount << "\n";

  return 0;
}