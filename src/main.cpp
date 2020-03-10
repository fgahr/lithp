#include <fstream>
#include <iostream>
#include <string>

#include <lithp.hpp>

int run_repl(void) {
  // TODO
  return EXIT_SUCCESS;
}

int run_file(std::string filename) {
  // TODO
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  switch (argc) {
  case 1:
    return run_repl();
  case 2:
    return run_file(argv[1]);
  default:
    std::cerr << "Too many arguments: " + std::to_string(argc) << "\n";
    return EXIT_FAILURE;
  }
}
