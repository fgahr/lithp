#include <sstream>

#include "lib_data.hpp"
#include "lib_util.hpp"

namespace lithp::lib {

namespace data {
Object *feq(SlotArgs slots, RestArgs) {
  if (eq(ARG0, ARG1)) {
    return Boolean::True();
  }

  Type t = type_of(ARG0);
  if (t != type_of(ARG1)) {
    return Boolean::False();
  }

  switch (t) {
  case Type::Number:
    return Boolean::of(Number::eq(Number::cast(ARG0), Number::cast(ARG1)));
  default:
    return Boolean::False();
  }
}

Object *fcons(SlotArgs slots, RestArgs) { return cons(ARG0, ARG1); }

Object *flist(SlotArgs, RestArgs rest) { return List::of(rest); }

Object *flistp(SlotArgs slots, RestArgs) {
  if (List::is_instance(ARG0)) {
    return Boolean::True();
  }
  return Boolean::False();
}

Object *fsetcar(SlotArgs slots, RestArgs) {
  if (!List::is_instance(ARG0)) {
    std::ostringstream msg{"not a pair: "};
    ARG0->repr(msg);
    throw std::runtime_error{msg.str()};
  }

  List::cast(ARG0)->set_car(ARG1);
  return nil();
}

Object *fsetcdr(SlotArgs slots, RestArgs) {
  if (!List::is_instance(ARG0)) {
    std::ostringstream msg{"not a pair: "};
    ARG0->repr(msg);
    throw std::runtime_error{msg.str()};
  }

  List::cast(ARG0)->set_cdr(ARG1);
  return nil();
}

} // namespace data

void load_data(Environment &env) {
  env.set(SYM("cons"), FUN(2, false, data::fcons));
  env.set(SYM("list"), FUN(0, true, data::flist));
  env.set(SYM("pair?"), FUN(1, false, data::flistp));
  env.set(SYM("eq?"), FUN(2, false, data::feq));
  env.set(SYM("setcar!"), FUN(2, false, data::fsetcar));
  env.set(SYM("setcdr!"), FUN(2, false, data::fsetcdr));
}
} // namespace lithp::lib
