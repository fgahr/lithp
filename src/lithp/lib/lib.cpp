#include <lib/lib.hpp>
#include <lithp.hpp>

#include "lib_util.hpp"

using namespace lithp;

Object *feq(FnArgs args, RestArgs _) {
  if (ARG0 == nullptr || ARG1 == nullptr) {
    throw std::logic_error{"attempting to compare NULL pointer"};
  }

  if (ARG0 == ARG1) {
    return Boolean::True();
  }

  Type t = ARG0->type();
  if (t != ARG1->type()) {
    return Boolean::False();
  }

  switch (t) {
  case Type::Number:
    return Number::eq(Number::cast(ARG0), Number::cast(ARG1))
               ? Boolean::True()
               : Boolean::False();
  default:
    return Boolean::False();
  }
}

static Object *fcons(FnArgs args, RestArgs _) {
  return ConsCell::make(ARG0, ARG1);
}

static Object *flist(FnArgs _, RestArgs rest) {
  if (rest.empty()) {
    return Object::nil();
  }

  ConsCell *head = ConsCell::make(rest.front(), Object::nil());
  ConsCell *cur = head;
  for (size_t i = 1; i < rest.size(); i++) {
    cur->car = ConsCell::make(rest.at(i), Object::nil());
  }

  return head;
}

static Object *flistp(FnArgs args, RestArgs _) {
  if (ConsCell::is_instance(ARG0)) {
    return Boolean::True();
  }
  return Boolean::False();
}

namespace lithp::lib {
void load_stdlib(Environment &env) {
  env.set(Symbol::intern("cons"), Builtin::make(2, false, fcons));
  env.set(Symbol::intern("list"), Builtin::make(0, true, flist));
  env.set(Symbol::intern("pair?"), Builtin::make(1, false, flistp));
  env.set(Symbol::intern("eq?"), Builtin::make(2, false, feq));
}
} // namespace lithp::lib
