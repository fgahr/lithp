#include <unordered_map>

#include <object/builtin.hpp>
#include <object/symbol.hpp>

namespace lithp {

static std::unordered_map<Symbol *, Builtin *> builtins;

void Builtin::create(std::string name, fnative *fnat) {
  Symbol *sym = Symbol::intern(name);
  builtins.insert_or_assign(sym, new Builtin{fnat});
}

size_t Builtin::size() { return sizeof(Builtin); }

RefStream Builtin::refs() { return RefStream::empty(); }

void Builtin::repr(std::ostream &out) {
  char buffer[64] = {'\0'};
  sprintf(buffer, "<builtin#%p>", this);
  out << buffer;
}

Object *Builtin::copy_to(void *mem) {
  throw std::logic_error{"attempting to copy a builtin function"};
}

Object *Builtin::call(std::vector<Object *> args) { return native(args); }

} // namespace lithp
