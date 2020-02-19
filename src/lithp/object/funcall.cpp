#include <lithp.hpp>

namespace lithp {

runtime::Allocator *Funcall::allocator = nullptr;

Type Funcall::type() { return Type::Funcall; }

Object *Funcall::eval(Environment &env) {
  // TODO
  return Nil::nil();
}

void Funcall::repr(std::ostream &out) {
  // TODO
}

RefStream Funcall::refs() {
  return RefStream::concat(RefStream::of({(Object **)&func}), fargs->refs());
}

Funcall *Funcall::make(Function *func, List *fargs) {
  return HEAP_NEW(Funcall) Funcall{func, fargs};
}

Funcall::Funcall(Function *func, List *args) : func{func}, fargs{args} {}

Object *Funcall::copy_to(void *mem) { return new (mem) Funcall{func, fargs}; }
} // namespace lithp
