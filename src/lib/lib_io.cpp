#include <iostream>

#include "lib_io.hpp"
#include "lib_util.hpp"

namespace lithp::lib {
namespace io {
Object *echo(size_t nargs, Object **args) {
    if (nargs == 0) {
        std::cout << "\n";
        return nil();
    }
    std::cout << display(args[0]);
    for (size_t i = 1; i < nargs; i++) {
        std::cout << " " << display(args[i]);
    }
    std::cout << "\n";
    return nil();
}
} // namespace io

void load_io(Environment &env) {
    env.def(SYM("echo"), FUN(0, true, io::echo));
    // TODO
}
} // namespace lithp::lib
