#include <object/nil.hpp>

namespace lithp {

Nil Nil::instance{};

size_t Nil::size() { return 0; }

Type Nil::type() { return Type::Nil; }

Object *Nil::eval(Environment &) { return this; }

void Nil::repr(std::ostream &out) { out << "nil"; }

RefStream Nil::refs() { return RefStream::empty(); }

Nil *Nil::nil() { return &instance; }

bool Nil::eq(Nil *, Nil *) { return true; }

Nil *Nil::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Nil); }

bool Nil::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Nil); }

} // namespace lithp
