#include <lithp.hpp>

namespace lithp {
Lambda *Lambda::of(std::vector<Symbol *> args, Symbol *rest, Object *body) {
  if (args.size() > MAX_NUM_ARGS) {
    throw std::logic_error{"too many function arguments (" +
                           std::to_string(args.size()) + " > " +
                           std::to_string(MAX_NUM_ARGS) + ")"};
  }
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

Object *Lambda::copy_to(void *mem) {
  // TODO
  throw std::logic_error{"lambda copying not yet implemented"};
}

Object *Lambda::call(List *args) {
  // TODO
  return nil();
}

size_t Lambda::num_args() { return nargs; }

bool Lambda::takes_rest() {
  // TODO
  return true;
}

bool Lambda::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Function); }

bool Lambda::eq(Lambda *l1, Lambda *l2) { return l1 == l2; }

} // namespace lithp
