#include <lithp.hpp>

namespace lithp {
Lambda *Lambda::of(List *declaration) {
  // TODO
  return nullptr;
}

Lambda::~Lambda() {
  // TODO
}

void Lambda::repr(std::ostream &out) {
  char buffer[64] = {'\0'};
  sprintf(buffer, "<lambda#%p>", this);
  out << buffer;
}

RefStream Lambda::refs() {
  // TODO
  return RefStream::empty();
}

Object *Lambda::copy_to(void *) {
  throw std::logic_error{"lambda copying not yet implemented"};
}

Object *Lambda::call(size_t nargs, Object **args) {
  // TODO
  return nil();
}

size_t Lambda::num_slots() { return nslots; }

bool Lambda::takes_rest() {
  // TODO
  return true;
}

bool Lambda::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Function); }

bool Lambda::eq(Lambda *l1, Lambda *l2) { return l1 == l2; }

} // namespace lithp
