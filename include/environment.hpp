#ifndef __LITHP_ENVIRONMENT_H_
#define __LITHP_ENVIRONMENT_H_

#include "object.hpp"
#include "symbol.hpp"

namespace lithp {
class Environment {
public:
  Environment(Environment *parent = nullptr);
  void set(Symbol *sym, Object *obj);
  Object *lookup(Symbol *sym);
  Object *pull_up(Symbol *sym);
  RefStream refs();

private:
  Environment *parent;
  std::unordered_map<Symbol *, Object *> definitions;
};
}

#endif // __LITHP_ENVIRONMENT_H_
