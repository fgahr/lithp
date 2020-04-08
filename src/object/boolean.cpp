#include <lithp/lithp.hpp>

namespace lithp {
void Boolean::repr(std::ostream &out) {
  if (this == True()) {
    out << "true";
  } else {
    out << "false";
  }
}

RefStream Boolean::refs() { return RefStream::empty(); }

Boolean *Boolean::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Boolean); }

bool Boolean::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Boolean); }

Boolean *Boolean::True() { return &t; }

Boolean *Boolean::False() { return &f; }

Boolean *Boolean::of(bool val) {
  if (val) {
    return True();
  }
  return False();
}

Boolean Boolean::t{};
Boolean Boolean::f{};

Boolean::Boolean() {}
} // namespace lithp
