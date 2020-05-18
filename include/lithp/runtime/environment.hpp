#ifndef _LITHP_RUNTIME_ENVIRONMENT_H_
#define _LITHP_RUNTIME_ENVIRONMENT_H_

#include <unordered_map>

#include <lithp/object.hpp>
#include <lithp/util/refstream.hpp>

namespace lithp {

class Function;
namespace runtime {

class Environment : public Object {
  public:
    virtual bool heap_allocated(void) override = 0;
    virtual size_t size(void) override = 0;
    virtual Type type(void) override = 0;
    virtual void repr(std::ostream &out) override = 0;
    virtual RefStream refs(void) override = 0;
    virtual Object *copy_to(void *mem) override = 0;
    virtual void set(Symbol *sym, Object *obj) = 0;
    virtual void def(Symbol *sym, Object *obj) = 0;
    virtual Object *get(Symbol *sym) = 0;
    virtual bool knows(Symbol *sym) = 0;
    static Environment *get_global();
    static Environment *make_local(Environment *parent);
    virtual ~Environment() = default;
};
} // namespace runtime
} // namespace lithp

#endif // _LITHP_RUNTIME_ENVIRONMENT_H_
