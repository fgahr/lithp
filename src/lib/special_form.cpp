#include <unordered_map>

#include <lithp.hpp>

#include "lib_util.hpp"

#define SYM Symbol::intern

namespace lithp::special {

using SlotArgs = std::vector<Object *>;
using RestArgs = List *;

typedef Object *(*snative)(SlotArgs, RestArgs, Environment &env);

namespace {
struct Form {
  Symbol *sym;
  size_t nslots;
  bool has_rest;
  snative nat;
};
} // namespace

static std::vector<Form> builtins;

Object *squote(SlotArgs slots, RestArgs, Environment &) { return ARG0; }

Object *sif(SlotArgs slots, RestArgs rest, Environment &env) {
  if (is_true(eval(ARG0, env))) {
    return eval(ARG1, env);
  } else if (is_null(rest)) {
    return nil();
  } else if (!is_null(cdr(rest))) {
    throw std::runtime_error{"too many branches on if-expression: " +
                             std::to_string(1 + List::length(rest))};
  } else {
    return eval(car(rest), env);
  }
}

Object *sdefine(SlotArgs slots, RestArgs rest, Environment &env) {
  if (Symbol::is_instance(ARG0)) {
    if (!is_null(rest)) {
      throw std::runtime_error{"malformed definition"};
    }
    env.set(Symbol::cast(ARG0), eval(ARG1, env));
  } else if (List::is_instance(ARG0)) {
    List *decl = List::cast(ARG0);
    Symbol *sym = Symbol::cast(car(decl));
    List *lambda_form = cons(Symbol::intern("lambda"), cons(cdr(decl), rest));
    Object *lambda = eval(lambda_form, env);
    env.set(sym, lambda);
  } else {
    throw std::runtime_error{"malformed definition"};
  }
  return nil();
}

void init() {
  builtins.push_back(Form{SYM("quote"), 1, false, squote});
  builtins.push_back(Form{SYM("if"), 2, true, sif});
  builtins.push_back(Form{SYM("define"), 2, true, sif});
}

bool is_special(Symbol *sym) {
  if (builtins.empty()) {
    init();
  }
  for (auto bi : builtins) {
    if (bi.sym == sym) {
      return true;
    }
  }
  return false;
}

static RestArgs fill_slots(size_t nslots, SlotArgs &slots, List *args) {
  for (size_t i = 0; i < nslots; i++) {
    slots.push_back(car(args));
    args = List::cast(args->cdr());
  }
  return args;
}

Object *dispatch(Symbol *type, List *args, Environment &env) {
  if (builtins.empty()) {
    init();
  }
  for (auto bi : builtins) {
    if (bi.sym == type) {
      SlotArgs slots;
      RestArgs rest = fill_slots(bi.nslots, slots, args);
      return bi.nat(slots, rest, env);
    }
  }
  throw std::logic_error{"not a special form: " + to_string(type)};
}
} // namespace lithp::special
