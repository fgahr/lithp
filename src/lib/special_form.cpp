#include <unordered_map>

#include <lithp.hpp>

#include "lib_util.hpp"

#define SYM Symbol::intern

namespace lithp::special {
typedef Object *(*native)(List *args);
static std::unordered_map<Symbol *, native> builtins;

void init() {
  // TODO
}

bool is_special(Symbol *sym) {
  if (builtins.empty()) {
    init();
  }
  return builtins.find(sym) != builtins.end();
}

Object *dispatch(Symbol *type, List *args) {
  if (builtins.empty()) {
    init();
  }
  auto it = builtins.find(type);
  if (it == builtins.end()) {
    throw std::logic_error{"not a special form: " + to_string(type)};
  }
  return it->second(args);
}
} // namespace lithp::special
