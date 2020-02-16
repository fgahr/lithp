#include <object/nil.hpp>

namespace lithp {

Nil Nil::instance{};

size_t Nil::size() { return 0; }

Type Nil::type() { return Type::Nil; }

Object *Nil::eval(Environment &env) { return this; }

void Nil::repr(std::ostream &out) { out << "nil"; }

RefStream Nil::refs() { return RefStream::empty(); }

Object *Nil::copy_to(void *mem) {
  throw std::logic_error{"attempting to move nil"};
}

Nil *Nil::nil() { return &instance; }

bool Nil::eq(Nil *n1, Nil *n2) { return true; }

Nil *Nil::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Nil); }

bool Nil::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Nil); }

} // namespace lithp
