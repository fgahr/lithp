#include <iostream>
#include <sstream>

#include <lithp.hpp>

namespace lithp::runtime {
Environment::Environment(Environment *parent) : parent{parent} {}

void Environment::set(Symbol *sym, Object *obj) {
  // FIXME: Make different from def
  if (sym->self_evaluating()) {
    throw std::runtime_error{"self-evaluating symbol " + to_string(sym) +
                             " cannot be assigned to"};
  }
  definitions.insert_or_assign(sym, obj);
}

void Environment::def(Symbol *sym, Object *obj) {
  // FIXME: Make different from set
  if (sym->self_evaluating()) {
    throw std::runtime_error{"self-evaluating symbol " + to_string(sym) +
                             " cannot be assigned to"};
  }
  definitions.insert_or_assign(sym, obj);
}

Object *Environment::get(Symbol *sym) {
  auto found = definitions.find(sym);
  if (found != definitions.end()) {
    return found->second;
  }

  if (parent) {
    return parent->get(sym);
  } else {
    return nil();
  }
}

Function *Environment::get_fun(Symbol *sym) {
  Object *found = get(sym);
  if (Function::is_instance(found)) {
    return Function::cast(found);
  } else if (is_null(found)) {
    throw std::runtime_error{"no such function: " + to_string(sym)};
  } else {
    throw std::runtime_error{"not a function: " + to_string(found)};
  }
}

bool Environment::knows(Symbol *sym) {
  auto found = definitions.find(sym);
  return found != definitions.end();
}

RefStream Environment::refs() { return refs_of(definitions); }
} // namespace lithp::runtime
