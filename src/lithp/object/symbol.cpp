#include <unordered_set>

#include <environment.hpp>
#include <symbol.hpp>

namespace lithp {
namespace {
class SymbolTable {
public:
  const char *create_or_get(const std::string name) {
    auto it = symbols.insert(name);
    return it.first->c_str();
  }

private:
  std::unordered_set<std::string> symbols;
};
} // namespace

static SymbolTable symtab;

Symbol::Symbol(const char *name) : name{name} {}

Symbol::Symbol(std::string name) { this->name = intern(name); }

Object *Symbol::eval(Environment &env) {
  if (self_evaluating()) {
    return this;
  }
  return env.lookup(this);
}

void Symbol::repr(std::ostream &out) { out << name; }

RefStream Symbol::refs() { return RefStream::empty(); }

Object *Symbol::copy_to(void *mem) { return new (mem) Symbol{name}; }

bool Symbol::self_evaluating() { return name[0] == ':'; }

static const char *symbol_legal_chars =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-/_?!";

bool Symbol::is_valid(std::string_view name) {
  return !name.empty() &&
         name.find_first_not_of(symbol_legal_chars) == std::string_view::npos &&
         name.find_first_of("0123456789") != 0;
}

const char *Symbol::intern(std::string name) {
  if (!is_valid(name)) {
    throw std::invalid_argument{"illegal symbol name '" + name + "'"};
  }
  return symtab.create_or_get(name);
}

bool Symbol::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Symbol); }

Symbol *Symbol::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Symbol); }

bool Symbol::eq(Symbol *s1, Symbol *s2) { return s1->name == s2->name; }

} // namespace lithp
