/* I will write an program which shares int i=0 with two threads and I will spin 2 threads each thread simply tries to i++ (for 1 million times) and in main program I will print the final value of i

// IMP is OBSERVATION : does the final integer value would be twice of the number of iterations? (means 2 million)*/

#include <iostream>
#include <thread>

using namespace std;

void increment(int& i) {
  for (int j = 0; j < 1000000; ++j) {
    i++;
  }
}

int main() {
  int i = 0;

  thread t1(increment, ref(i));
  thread t2(increment, ref(i));

  t1.join();
  t2.join();

  cout << "Final value of i: " << i << endl;
  return 0;
}
