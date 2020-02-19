#include <object/boolean.hpp>

namespace lithp {
Object *Boolean::eval(Environment &) { return this; }

void Boolean::repr(std::ostream &out) {
  if (value) {
    out << "true";
  } else {
    out << "false";
  }
}

RefStream Boolean::refs() { return RefStream::empty(); }

Boolean *Boolean::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Boolean); }

bool Boolean::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Boolean); }

bool Boolean::eq(Boolean *n1, Boolean *n2) { return n1 == n2; }

Boolean *Boolean::True() {
  static Boolean t{true};
  return &t;
}

Boolean *Boolean::False() {
  static Boolean f{false};
  return &f;
}

Boolean *Boolean::of(bool val) {
  if (val) {
    return True();
  }
  return False();
}

Boolean::Boolean(bool value) : value{value} {}
} // namespace lithp
