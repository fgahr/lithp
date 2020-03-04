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
  if (nargs < nslots) {
    throw std::runtime_error{
        "not enough arguments to call function: " + std::to_string(nargs) +
        " (" + std::to_string(nslots) + " required)"};
  } else if (nargs > nslots && !has_rest) {
    throw std::runtime_error{
        "too many arguments for function call: " + std::to_string(nargs) +
        " (" + std::to_string(nslots) + " required)"};
  }

  return native(nargs, args);
}

size_t Builtin::num_slots() { return nslots; }
bool Builtin::takes_rest() { return has_rest; }

} // namespace lithp
