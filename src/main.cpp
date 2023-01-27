#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>


class Scanner {
public:
  void outputSourceString();
  void readSourceFile(const std::string &fileName);
private:
  std::string sourceFile;
};

void Scanner::outputSourceString() {
  std::cout << sourceFile << '\n';
}

void Scanner::readSourceFile(const std::string &fileName) {
  std::ifstream file(fileName);

  if (!file) {
    throw std::runtime_error("Failed to read file ");
  }

  while(file) {
    std::string inputStream;
    std::getline(file, inputStream);
    sourceFile += inputStream;
  }
}

void argumentError(int argc) {
  std::cout << "Incorrect amount of arguments!" << '\n';
}

int main(int argc, char* argv[]) {
  if (argc <= 1 || argc > 2) {
    argumentError(argc);
    return 1;
  }

  Scanner s{};

  try {
    s.readSourceFile(argv[1]);
  } catch (std::runtime_error e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  s.outputSourceString();

  return 0;
}
