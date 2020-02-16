#include <lithp.hpp>

namespace lithp {

runtime::Allocator *ConsCell::allocator = nullptr;

ConsCell::ConsCell(Object *car, Object *cdr) : car{car}, cdr{cdr} {}

Object *ConsCell::eval(Environment &env) {
  throw std::logic_error{"ConsCell cannot evaluate itself"};
}

void ConsCell::repr(std::ostream &out) {
  // TODO: Figure out what to do here: print entire list or throw instead?
  // Something else?
}

RefStream ConsCell::refs() {
  return RefStream::concat(car->refs(), cdr->refs());
}

Object *ConsCell::copy_to(void *mem) { return new (mem) ConsCell{car, cdr}; }

bool ConsCell::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, ConsCell); }

ConsCell *ConsCell::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, ConsCell); }

bool ConsCell::eq(ConsCell *c1, ConsCell *c2) { return c1 == c2; }

ConsCell *ConsCell::make(Object *car, Object *cdr) {
  return new (allocator->allocate(sizeof(ConsCell))) ConsCell{car, cdr};
}
} // namespace lithp
