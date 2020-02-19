#include <lithp.hpp>

namespace lithp {

runtime::Allocator *ConsCell::allocator = nullptr;

ConsCell::ConsCell(Object *car, Object *cdr) : car{car}, cdr{cdr} {}

Object *ConsCell::eval(Environment &) {
  throw std::logic_error{"ConsCell cannot evaluate itself"};
}

void ConsCell::repr(std::ostream &out) {
  out << "(";
  car->repr(out);
  Object *rest = cdr;
  while (ConsCell::is_instance(rest)) {
    out << " ";
    ConsCell *as_list = ConsCell::cast(rest);
    as_list->car->repr(out);
    rest = as_list->cdr;
  }

  if (Nil::is_instance(rest)) {
    out << ")";
  } else {
    out << " . ";
    rest->repr(out);
    out << ")";
  }
}

RefStream ConsCell::refs() {
  return RefStream::concat(RefStream::of({&car, &cdr}),
                           RefStream::concat(car->refs(), cdr->refs()));
}
Object *ConsCell::copy_to(void *mem) { return new (mem) ConsCell{car, cdr}; }

Object *ConsCell::head() { return car; }
Object *ConsCell::tail() { return cdr; }
void ConsCell::set_car(Object *obj) { car = obj; }
void ConsCell::set_cdr(Object *obj) { cdr = obj; }
Object *ConsCell::get_car() { return car; }
Object *ConsCell::get_cdr() { return cdr; }

bool ConsCell::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, ConsCell); }
ConsCell *ConsCell::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, ConsCell); }

ConsCell *ConsCell::make(Object *car, Object *cdr) {
  return HEAP_NEW(ConsCell) ConsCell{car, cdr};
}
} // namespace lithp
