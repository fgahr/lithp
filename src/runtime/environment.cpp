#include <sstream>

#include <lithp.hpp>

namespace lithp::runtime {
Environment::Environment(Environment *parent) : parent{parent} {}

void Environment::set(Symbol *sym, Object *obj) {
  // NOTE: Check whether symbol is self-evaluating cannot be done here.
  // Hence it needs to be done beforehand.
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

// TODO: Might make no sense to have this, delete?
Object *Environment::pull_up(Symbol *sym) {
  Object *obj = get(sym);
  definitions.insert_or_assign(sym, obj);
  return obj;
}

RefStream Environment::refs() { return refs_of(definitions); }
} // namespace lithp::runtime
