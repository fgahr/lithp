#include <unordered_map>

#include <lithp.hpp>

#include "lib_util.hpp"

#define SYM Symbol::intern

namespace lithp::special {
typedef Object *(*snative)(List *args, Environment &env);
static std::unordered_map<Symbol *, snative> builtins;

Object *squote(List *args, Environment &) { return args->car(); }

Object *sif(List *args, Environment &env) {
  // FIXME: Will fail with cyclic lists.
  size_t alen = List::length(args);
  if (alen < 2 || alen > 3) {
    throw std::runtime_error{"invalid if-form: " +
                             to_string(cons(SYM("if"), args))};
  }
  if (is_true(eval(car(args), env))) {
    return eval(nth(1, args), env);
  } else if (alen == 3) {
    return eval(nth(2, args), env);
  } else {
    return nil();
  }
}

  void init() {
    builtins.insert_or_assign(SYM("quote"), squote);
    builtins.insert_or_assign(SYM("if"), sif);
  }

bool is_special(Symbol *sym) {
  if (builtins.empty()) {
    init();
  }
  return builtins.find(sym) != builtins.end();
}

Object *dispatch(Symbol *type, List *args, Environment &env) {
  if (builtins.empty()) {
    init();
  }
  auto it = builtins.find(type);
  if (it == builtins.end()) {
    throw std::logic_error{"not a special form: " + to_string(type)};
  }
  return it->second(args, env);
}
} // namespace lithp::special
