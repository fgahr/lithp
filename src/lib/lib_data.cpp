#include <sstream>

#include <lithp.hpp>

#include "lib_data.hpp"
#include "lib_util.hpp"

using namespace lithp::runtime;

namespace lithp::lib {

using StackRef = runtime::stack::Ref;

namespace data {
Object *feq(size_t, Object **args) { return Boolean::of(eq(args[0], args[1])); }

Object *fnull(size_t, Object **args) { return Boolean::of(is_null(args[0])); }
Object *fcar(size_t, Object **args) {
  if (is_null(args[0])) {
    throw std::runtime_error{"not a list: " + to_string(args[0])};
  }
  return car(List::cast(args[0]));
}
Object *fcdr(size_t, Object **args) {
  if (is_null(args[0])) {
    throw std::runtime_error{"not a list: " + to_string(args[0])};
  }
  return cdr(List::cast(args[0]));
}
Object *fnth(size_t, Object **args) {
  auto n = Number::cast(args[0])->int_value();
  if (n < 0) {
    throw std::runtime_error{"no such element: " + std::to_string(n)};
  }
  return nth(n, List::cast(args[1]));
}

Object *fcons(size_t, Object **args) { return cons(args[0], args[1]); }

Object *flist(size_t nargs, Object **args) {
  if (nargs == 0) {
    return nil();
  }

  List *head = List::make(args[0], nil());
  StackRef href = stack::push(head);
  List *current = List::cast(stack::get(href));
  StackRef cref = stack::push(current);
  for (size_t i = 1; i < nargs; i++) {
    StackRef nref = stack::push(List::make(args[i], nil()));
    current = List::cast(stack::get(cref));
    current->set_cdr(stack::get(nref));
    stack::pop(); // nref
    stack::set(cref, cdr(current));
  }
  return runtime::stack::get(href);
}

Object *flistp(size_t, Object **args) {
  if (List::is_instance(args[0])) {
    return Boolean::True();
  }
  return Boolean::False();
}

Object *fsetcar(size_t, Object **args) {
  if (!List::is_instance(args[0])) {
    throw std::runtime_error{"not a pair: " + to_string(args[0])};
  }

  List::cast(args[0])->set_car(args[1]);
  return nil();
}

Object *fsetcdr(size_t, Object **args) {
  if (!List::is_instance(args[0])) {
    throw std::runtime_error{"not a pair: " + to_string(args[0])};
  }

  List::cast(args[0])->set_cdr(args[1]);
  return nil();
}

} // namespace data

void load_data(Environment &env) {
  env.def(SYM("eq?"), FUN(2, false, data::feq));
  env.def(SYM("null?"), FUN(1, false, data::fnull));
  env.def(SYM("car"), FUN(1, false, data::fcar));
  env.def(SYM("cdr"), FUN(1, false, data::fcdr));
  env.def(SYM("nth"), FUN(2, false, data::fnth));

  env.def(SYM("cons"), FUN(2, false, data::fcons));
  env.def(SYM("list"), FUN(0, true, data::flist));
  env.def(SYM("pair?"), FUN(1, false, data::flistp));
  env.def(SYM("set-car!"), FUN(2, false, data::fsetcar));
  env.def(SYM("set-cdr!"), FUN(2, false, data::fsetcdr));
}
} // namespace lithp::lib
