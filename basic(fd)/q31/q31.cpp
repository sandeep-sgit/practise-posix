// Write program to read more than exists- and see what happens

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

using namespace std;

int main() {
  // 1. open the file
  int fd = open("test.txt", O_RDONLY);
  if (fd == -1) {
    cerr << "Error opening file" << endl;
    return 1;
  }

  // 2. create a buffer to read data into
  char buffer[100];  // small buffer to read more than exists

  // 3. read data from the file
  ssize_t bytesRead = read(fd, buffer, sizeof(buffer));

  // 4. check the result of the read operation
  if (bytesRead == -1) {
    cerr << "Error reading file" << endl;
    close(fd);
    return 1;
  } else if (bytesRead == 0) {
    cout << "End of file reached" << endl;
  } else {
    cout << "Read " << bytesRead << " bytes from the file." << endl;
    cout.write(buffer, bytesRead);  // write the read data to stdout
    cout << endl;
  }
}
