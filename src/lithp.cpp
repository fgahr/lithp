#include <lithp.hpp>

namespace lithp {
Object *nil() { return nullptr; }
bool is_null(Object *obj) { return obj == nullptr; }
bool is_pair(Object *obj) { return List::is_instance(obj); }
bool eq(Object *o1, Object *o2) { return o1 == o2; }

Type type_of(Object *obj) {
  if (obj) {
    return obj->type();
  }
  return Type::Nil;
}

Object *eval(Object *obj, Environment &env) {
  if (is_null(obj)) {
    return nil();
  }
  return obj->evaluate(env);
}

Object *apply(Function *fun, List *args) {
  SlotArgs slots;

  for (size_t slot = 0; slot < fun->num_slots(); slot++) {
    if (args == nullptr) {
      throw std::runtime_error{
          "not enough arguments for function call: " + std::to_string(slot) +
          "; required: " + std::to_string(fun->num_slots())};
    }
    slots.push_back(args->car());
    args = List::cast(args->cdr());
  }

  RestArgs rest = List::flatten(args);

  return fun->call(slots, rest);
}

List *cons(Object *car, Object *cdr) { return List::make(car, cdr); }
std::string to_string(Object *obj) {
  std::ostringstream out;
  obj->repr(out);
  return out.str();
}
} // namespace lithp
