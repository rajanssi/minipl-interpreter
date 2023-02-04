#include <memory>
#include "Scanner.h"

inline void argumentError(int argc) {
  if (argc == 1) {
    std::cout << "Please pass in the source file to interpret!\n";
  } else {
    std::cout << "Too many arguments, only pass in the source file to interpret!\n";
  }
}

int main(int argc, char *argv[]) {
  if (argc <= 1 || argc > 2) {
    argumentError(argc);
    return 1;
  }

  std::unique_ptr<Scanner> scanner;

  try {
    scanner = std::make_unique<Scanner>(argv[1]);
  } catch (const std::runtime_error &e) {
    std::cerr << "Error trying to create Scanner: " << e.what() << '\n';
    return 1;
  }

  scanner->scanSource();
  // NOTE: Print tokens for show
  for (auto t : scanner->tokens) {
    t.print();
    std::cout << '\n';
  }

  return 0;
}
