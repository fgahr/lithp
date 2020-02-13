#include <environment.hpp>
#include <object/symbol.hpp>

namespace lithp {
class SymbolChain {
public:
  SymbolChain(std::string name) : sym{name} {}
  Symbol *find_or_add(std::string name) {
    if (sym.name == name) {
      return &sym;
    } else if (next) {
      return next->find_or_add(name);
    } else {
      next = new SymbolChain(name);
      return &(next->sym);
    }
  }
  SymbolChain *next = nullptr;

private:
  Symbol sym;
};

namespace {

class ChainHead {
public:
  Symbol *find_or_add(std::string name) {
    if (chain) {
      return chain->find_or_add(name);
    }
    chain = new SymbolChain{name};
    return chain->find_or_add(name);
  }
  ~ChainHead() {
    while (chain) {
      SymbolChain *rest = chain->next;
      delete chain;
      chain = rest;
    }
  }

private:
  SymbolChain *chain = nullptr;
};

#define LITHP_SYMBOL_TABLE_SIZE 1024

class SymbolTable {
public:
  SymbolTable() : size{LITHP_SYMBOL_TABLE_SIZE} { symbols.resize(size); }
  Symbol *create_or_get(const std::string name) {
    size_t h = std::hash<std::string_view>{}(name);
    return symbols.at(h % size).find_or_add(name);
  }

private:
  size_t size;
  std::vector<ChainHead> symbols;
};
} // namespace

static SymbolTable symtab;

Symbol::Symbol(std::string name) : name{std::move(name)} {}

Object *Symbol::eval(Environment &env) {
  if (self_evaluating()) {
    return this;
  }
  return env.lookup(this);
}

void Symbol::repr(std::ostream &out) { out << name; }

RefStream Symbol::refs() { return RefStream::empty(); }

Object *Symbol::copy_to(void *mem) { return new (mem) Symbol{name}; }

bool Symbol::self_evaluating() { return name.at(0) == ':'; }

std::string Symbol::get_name() { return name; }

static const char *symbol_legal_chars =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:+-/_?!";

bool Symbol::is_valid(std::string_view name) {
  return !name.empty() &&
         name.find_first_not_of(symbol_legal_chars) == std::string_view::npos &&
         name.find_first_of("0123456789") != 0;
}

Symbol *Symbol::intern(std::string name) {
  if (!is_valid(name)) {
    throw std::invalid_argument{"illegal symbol name '" + name + "'"};
  }
  return symtab.create_or_get(name);
}

bool Symbol::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Symbol); }

Symbol *Symbol::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Symbol); }

bool Symbol::eq(Symbol *s1, Symbol *s2) { return s1->name == s2->name; }

} // namespace lithp
