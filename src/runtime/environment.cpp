#include <iostream>
#include <sstream>

#include <lithp.hpp>

namespace lithp::runtime {
Environment::Environment(Environment *parent) : parent{parent} {}

void Environment::set(Symbol *sym, Object *obj) {
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
  std::cerr << "looking up " + to_string(sym) << "\n";
  Object *found = get(sym);
  std::cerr << "found " + to_string(sym) << "\n";
  if (Function::is_instance(found)) {
    return Function::cast(found);
  } else if (is_null(found)) {
    throw std::runtime_error{"no such function: " + to_string(sym)};
  } else {
    throw std::runtime_error{"not a function: " + to_string(found)};
  }
}

RefStream Environment::refs() { return refs_of(definitions); }
} // namespace lithp::runtime
