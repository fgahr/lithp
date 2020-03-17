#include <sstream>

#include <lithp.hpp>

#include "lib_data.hpp"
#include "lib_util.hpp"

using namespace lithp::runtime;

namespace lithp::lib {

using StackRef = runtime::stack::Ref;

namespace data {
Object *eq(size_t, Object **args) { return Boolean::of(eq(args[0], args[1])); }
Object *null(size_t, Object **args) { return Boolean::of(is_null(args[0])); }
Object *car(size_t, Object **args) {
  if (is_null(args[0])) {
    throw std::runtime_error{"not a list: " + to_string(args[0])};
  }
  return car(List::cast(args[0]));
}
Object *cdr(size_t, Object **args) {
  if (is_null(args[0])) {
    throw std::runtime_error{"not a list: " + to_string(args[0])};
  }
  return cdr(List::cast(args[0]));
}
Object *nth(size_t, Object **args) {
  auto n = Number::cast(args[0])->int_value();
  if (n < 0) {
    throw std::runtime_error{"no such element: " + std::to_string(n)};
  }
  return nth(n, List::cast(args[1]));
}

Object *cons(size_t, Object **args) { return cons(args[0], args[1]); }

Object *list(size_t nargs, Object **args) { return List::of(nargs, args); }

Object *listp(size_t, Object **args) {
  if (List::is_instance(args[0])) {
    return Boolean::True();
  }
  return Boolean::False();
}

Object *setcar(size_t, Object **args) {
  if (!List::is_instance(args[0])) {
    throw std::runtime_error{"not a pair: " + to_string(args[0])};
  }

  List::cast(args[0])->set_car(args[1]);
  return args[0];
}

Object *setcdr(size_t, Object **args) {
  if (!List::is_instance(args[0])) {
    throw std::runtime_error{"not a pair: " + to_string(args[0])};
  }

  List::cast(args[0])->set_cdr(args[1]);
  return args[0];
}
} // namespace data

void load_data(Environment &env) {
  env.def(SYM("eq?"), FUN(2, false, data::eq));
  env.def(SYM("null?"), FUN(1, false, data::null));
  env.def(SYM("car"), FUN(1, false, data::car));
  env.def(SYM("cdr"), FUN(1, false, data::cdr));
  env.def(SYM("nth"), FUN(2, false, data::nth));

  env.def(SYM("cons"), FUN(2, false, data::cons));
  env.def(SYM("list"), FUN(0, true, data::list));
  env.def(SYM("pair?"), FUN(1, false, data::listp));
  env.def(SYM("set-car!"), FUN(2, false, data::setcar));
  env.def(SYM("set-cdr!"), FUN(2, false, data::setcdr));
}
} // namespace lithp::lib
