#ifndef _LITHP_RUNTIME_ENVIRONMENT_H_
#define _LITHP_RUNTIME_ENVIRONMENT_H_

#include <unordered_map>

#include <lithp/util/refstream.hpp>

namespace lithp {

class Function;
namespace runtime {

class Environment {
  public:
    Environment(Environment *parent = nullptr);
    void set(Symbol *sym, Object *obj);
    void def(Symbol *sym, Object *obj);
    Object *get(Symbol *sym);
    bool knows(Symbol *sym);
    RefStream refs();

  private:
    Environment *parent = nullptr;
    std::unordered_map<Symbol *, Object *> definitions{};
};
} // namespace runtime
} // namespace lithp

#endif // _LITHP_RUNTIME_ENVIRONMENT_H_
