#ifndef _LITHP_RUNTIME_ENVIRONMENT_H_
#define _LITHP_RUNTIME_ENVIRONMENT_H_

#include <unordered_map>

#include <util/refstream.hpp>

namespace lithp {

class Function;
namespace runtime {

class Environment {
public:
  Environment(Environment *parent = nullptr);
  void set(Symbol *sym, Object *obj);
  Object *get(Symbol *sym);
  Function *get_fun(Symbol *sym);
  RefStream refs();

private:
  Environment *parent = nullptr;
  std::unordered_map<Symbol *, Object *> definitions{};
};
} // namespace runtime
} // namespace lithp

#endif // _LITHP_RUNTIME_ENVIRONMENT_H_
