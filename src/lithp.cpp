#include <sstream>

#include <lithp.hpp>
#include <reader/reader.hpp>

namespace lithp {
Object *nil() { return nullptr; }
bool is_null(Object *obj) { return obj == nullptr; }
bool is_pair(Object *obj) { return List::is_instance(obj); }

List *cons(Object *car, Object *cdr) { return List::make(car, cdr); }
std::string to_string(Object *obj) {
  if (is_null(obj)) {
    return "nil";
  } else {
    std::ostringstream out;
    obj->repr(out);
    return out.str();
  }
}

Object *car(List *list) {
  if (is_null(list)) {
    return nil();
  }
  return list->car();
}

Object *cdr(List *list) {
  if (is_null(list)) {
    return nil();
  }
  return list->cdr();
}

Object *nth(size_t n, List *list) {
  for (size_t i = 0; i < n; list = List::cast(cdr(list))) {
    ++i;
  }
  return car(list);
}

bool eq(Object *o1, Object *o2) {
  if (o1 == o2) {
    return true;
  } else if (type_of(o1) != type_of(o2)) {
    return false;
  }

  switch (type_of(o1)) {
  case Type::Number:
    return Number::eq(Number::cast(o1), Number::cast(o2));
  default:
    return false;
  }
}

Type type_of(Object *obj) {
  if (obj) {
    return obj->type();
  }
  return Type::Nil;
}

bool is_true(Object *obj) {
  if (is_null(obj) || obj == Boolean::False()) {
    return false;
  }
  return true;
}
bool is_false(Object *obj) { return !is_true(obj); }

Object *eval(Object *obj, Environment &env) {
  if (is_null(obj)) {
    return nil();
  }
  return obj->evaluate(env);
}

Object *eval_sequence(List *seq, Environment &env) {
  Object *result = nil();
  for (List *rem = seq; !is_null(rem); rem = List::cast(rem->cdr())) {
    result = eval(rem->car(), env);
  }
  return result;
}

Object *apply(Function *fun, List *args, Environment &env) {
  if (is_null(fun)) {
    throw std::runtime_error{"not a function: " + to_string(fun)};
  }
  SlotArgs slots;

  List *rest = args;
  for (size_t slot = 0; slot < fun->num_slots(); slot++) {
    if (is_null(args)) {
      throw std::runtime_error{
          "not enough arguments for function call: " + std::to_string(slot) +
          "; required: " + std::to_string(fun->num_slots())};
    }
    slots.push_back(eval(rest->car(), env));
    rest = List::cast(cdr(rest));
  }

  return fun->call(slots, args);
}

} // namespace lithp
