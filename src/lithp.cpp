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
List *cons(Object *car, Object *cdr) { return List::make(car, cdr); }
std::string to_string(Object *obj) {
  std::ostringstream out;
  obj->repr(out);
  return out.str();
}
} // namespace lithp
