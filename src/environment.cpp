#include <sstream>

#include "environment.hpp"

namespace lithp {
Environment::Environment(Environment *parent) : parent{parent} {}

void Environment::set(Symbol *sym, Object *obj) {
  if (sym->self_evaluating()) {
    throw std::invalid_argument{"self-evaluating symbol " +
                                std::string{sym->name} +
                                " cannot be assigned to"};
  }
  definitions.insert_or_assign(sym, obj);
}

Object *Environment::lookup(Symbol *sym) {
  auto found = definitions.find(sym);
  if (found != definitions.end()) {
    return found->second;
  }

  if (parent) {
    return parent->lookup(sym);
  } else {
    std::ostringstream error{"unknown symbol "};
    error << sym->name;
    throw std::invalid_argument{error.str()};
  }
}

Object *Environment::pull_up(Symbol *sym) {
  Object *obj = lookup(sym);
  definitions.insert_or_assign(sym, obj);
  return obj;
}

RefStream Environment::refs() { return refs_of(definitions); }
}