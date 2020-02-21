#include <unordered_map>

#include <lithp.hpp>

#include "lib_util.hpp"

#define SYM Symbol::intern

namespace lithp::special {
typedef Object *(*snative)(List *args);
static std::unordered_map<Symbol *, snative> builtins;

Object *squote(List *args) { return args; }

void init() { builtins.insert_or_assign(SYM("quote"), squote); }

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
