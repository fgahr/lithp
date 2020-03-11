#include <fstream>
#include <iostream>
#include <string>

#include <lithp.hpp>
#include <reader/reader.hpp>

const std::string prompt = "> ";

int run_repl(void) {
  lithp::runtime::init();
  lithp::reader::init(std::cin);
  try {
    while (!lithp::reader::done()) {
      std::cout << prompt;
      lithp::Object *read = lithp::reader::next_expr();
      std::cout << lithp::to_string(eval(read, lithp::runtime::global_env()))
                << "\n";
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int run_file(std::string) {
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
