#include <unordered_map>

#include <object/builtin.hpp>
#include <object/symbol.hpp>

namespace lithp {

static std::unordered_map<Symbol *, Builtin *> builtins;

Builtin::Builtin(size_t nargs, bool rest, fnative fnat)
    : nslots{nargs}, has_rest{rest}, native{fnat} {}

Builtin *Builtin::make(size_t nargs, bool rest, fnative *fnat) {
  return new Builtin{nargs, rest, fnat};
}

size_t Builtin::size() { return sizeof(Builtin); }

RefStream Builtin::refs() { return RefStream::empty(); }

void Builtin::repr(std::ostream &out) {
  char buffer[64] = {'\0'};
  sprintf(buffer, "<builtin#%p>", this);
  out << buffer;
}

Object *Builtin::copy_to(void *) {
  throw std::logic_error{"attempting to copy a builtin function"};
}

Object *Builtin::call(size_t nargs, Object **args) {
  // TODO
  return nullptr;
}

size_t Builtin::num_slots() { return nslots; }
bool Builtin::takes_rest() { return has_rest; }

} // namespace lithp
