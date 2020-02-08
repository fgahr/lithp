#include "cons_cell.hpp"

namespace lithp {
ConsCell::ConsCell(Object *car, Object *cdr) : car{car}, cdr{cdr} {}

Object *ConsCell::eval(Environment &env) {
  throw std::logic_error{"ConsCell cannot evaluate itself"};
}

RefStream ConsCell::refs() {
  // TODO: Check for nil
  return RefStream::concat(car->refs(), cdr->refs());
}

bool ConsCell::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, ConsCell); }

ConsCell *ConsCell::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, ConsCell); }
} // namespace lithp
