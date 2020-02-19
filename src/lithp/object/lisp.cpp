#include <lithp.hpp>

namespace lithp {

runtime::Allocator *List::allocator = nullptr;

List::List(Object *car, Object *cdr) : car{car}, cdr{cdr} {}

Object *List::eval(Environment &) {
  throw std::logic_error{"ConsCell cannot evaluate itself"};
}

void List::repr(std::ostream &) {
  // TODO: Figure out what to do here: print entire list or throw instead?
  // Something else?
}

RefStream List::refs() {
  return RefStream::concat(car->refs(), cdr->refs());
}

Object *List::copy_to(void *mem) { return new (mem) List{car, cdr}; }

bool List::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, List); }

List *List::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, List); }

List *List::make(Object *car, Object *cdr) {
  return HEAP_NEW(List) List{car, cdr};
}
} // namespace lithp
