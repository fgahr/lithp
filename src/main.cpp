#include <fstream>
#include <iostream>
#include <string>

#include <lithp.hpp>
#include <reader/reader.hpp>

const std::string prompt = "> ";

int run_repl(void) {
  lithp::runtime::init();
  lithp::reader::init(std::cin);
  while (!lithp::reader::done()) {
    try {
      std::cout << prompt;
      lithp::Object *expr = lithp::reader::next_expr();
      std::cout << lithp::to_string(eval(expr, lithp::runtime::global_env()))
                << "\n";
    } catch (const std::exception &e) {
      std::cerr << e.what() << "\n";
    }
  }

  return EXIT_SUCCESS;
}

void detect_and_remove_shebang(std::istream &infile) {
  char c;
  infile.get(c);
  if (c != '#') {
    infile.unget();
    return;
  }

  infile.get(c);
  if (c != '!') {
    infile.unget();
    return;
  }

  // Remove shebang line
  std::string throwaway;
  std::getline(infile, throwaway);
}

int run_file(std::string filename) {
  std::fstream infile{filename};
  detect_and_remove_shebang(infile);

  lithp::runtime::init();
  lithp::reader::init(infile);
  try {
    while (!lithp::reader::done()) {
      lithp::Object *expr = lithp::reader::next_expr();
      eval(expr, lithp::runtime::global_env());
    }
  } catch (const std::exception &e) {
    return EXIT_FAILURE;
  }
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
