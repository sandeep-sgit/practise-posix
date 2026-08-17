/* Write program to copy a file
    These is executable which receives 3 arguments - source, destination & buffer size.
    simply copy the source to destination*/

// use writeall instead of write to handle partial writes.

// Also print no of iteration taken and how much buffer size used in iteration.
#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc != 4) {  // 1. Argument count check
    std::cerr << "Usage: " << argv[0] << " <source> <destination> <buffer_size>\n";
    return 1;
  }

  const std::string source = argv[1];
  const std::string destination = argv[2];
  const size_t bufferSize = std::stoul(argv[3]);
  // Validate buffer size valid integer or not
  if (bufferSize <= 0) {
    std::cerr << "Invalid buffer size. Please provide a valid positive integer.\n";
    return 1;
  }

  int sourceFd = open(source.c_str(), O_RDONLY);  // 2. Open source file for reading
  if (sourceFd == -1) {
    perror("open source");
    return 1;
  }

  int destinationFd = open(destination.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);  // 3. Open destination file for writing (create if it doesn't exist, truncate if it does)
  if (destinationFd == -1) {
    perror("open destination");
    close(sourceFd);
    return 1;
  }

  char* buffer = new char[bufferSize];  // 4. Buffer to hold data read from source file

  ssize_t bytesRead;
  size_t totalBytesRead = 0;
  size_t iterationCount = 0;

  while ((bytesRead = read(sourceFd, buffer, bufferSize)) > 0) {  // 5. Read from source file into buffer
    ssize_t bytesWritten = writeall(destinationFd, buffer, bytesRead);

    if (bytesWritten == -1) {
      perror("write");
      close(sourceFd);
      close(destinationFd);
      return 1;
    }

    iterationCount++;
    totalBytesRead += bytesRead;
  }

  delete[] buffer;  // Free the allocated buffer memory

  if (bytesRead == -1) {
    perror("read");
    close(sourceFd);
    close(destinationFd);
    return 1;
  }

  close(sourceFd);
  close(destinationFd);

  std::cout << "Iterations: " << iterationCount << '\n';
  std::cout << "Bytes copied: " << totalBytesRead << '\n';
}