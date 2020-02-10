#include <object/cons_cell.hpp>

namespace lithp {
ConsCell::ConsCell(Object *car, Object *cdr) : car{car}, cdr{cdr} {}

Object *ConsCell::eval(Environment &env) {
  throw std::logic_error{"ConsCell cannot evaluate itself"};
}

void ConsCell::repr(std::ostream &out) {
  // TODO
}

RefStream ConsCell::refs() {
  // TODO: Check for nil
  return RefStream::concat(car->refs(), cdr->refs());
}

Object *ConsCell::copy_to(void *mem) {
  // TODO
  throw std::logic_error{"copying ConsCell not yet implemented"};
}

bool ConsCell::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, ConsCell); }

ConsCell *ConsCell::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, ConsCell); }

bool ConsCell::eq(ConsCell *c1, ConsCell *c2) { return c1 == c2; }
} // namespace lithp
