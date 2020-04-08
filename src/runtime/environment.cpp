#include <iostream>
#include <sstream>

#include <lithp/lithp.hpp>

namespace lithp::runtime {
Environment::Environment(Environment *parent) : parent{parent} {}

static void ensure_settable(Symbol *sym) {
  if (sym == nullptr) {
    throw std::runtime_error{"symbol is NULL"};
  } else if (sym->self_evaluating()) {
    throw std::runtime_error{"self-evaluating symbol " + to_string(sym) +
                             " cannot be assigned to"};
  }
}

void Environment::set(Symbol *sym, Object *obj) {
  ensure_settable(sym);

  if (knows(sym)) {
    definitions.insert_or_assign(sym, obj);
  } else if (parent == nullptr) {
    throw std::runtime_error{"cannot set unknown variable: " + sym->get_name()};
  } else {
    parent->set(sym, obj);
  }
}

void Environment::def(Symbol *sym, Object *obj) {
  ensure_settable(sym);

  if (knows(sym)) {
    throw std::runtime_error{"double definition of variable: " +
                             sym->get_name()};
  } else {
    definitions[sym] = obj;
  }
}

Object *Environment::get(Symbol *sym) {
  auto found = definitions.find(sym);
  if (found != definitions.end()) {
    return found->second;
  }

  if (parent) {
    return parent->get(sym);
  } else {
    throw std::runtime_error{"undefined variable: " + sym->get_name()};
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
