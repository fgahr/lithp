#include <sstream>

#include <object/nil.hpp>
#include <runtime/environment.hpp>

namespace lithp::runtime {
Environment::Environment(Environment *parent) : parent{parent} {}

void Environment::set(Symbol *sym, Object *obj) {
  // NOTE: Check whether symbol is self-evaluating cannot be done here.
  // Hence it needs to be done beforehand.
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
    return (Object *)(Nil::nil());
  }
}

Object *Environment::pull_up(Symbol *sym) {
  Object *obj = lookup(sym);
  definitions.insert_or_assign(sym, obj);
  return obj;
}

RefStream Environment::refs() { return refs_of(definitions); }
} // namespace lithp::runtime
