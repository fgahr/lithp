#ifndef _LITHP_OBJECT_BUILTIN_H_
#define _LITHP_OBJECT_BUILTIN_H_

#include <unordered_map>

#include <lithp/object/function.hpp>
#include <lithp/object/symbol.hpp>

namespace lithp {

typedef Object *(fnative)(size_t, Object **);

class Builtin : public Function {
  public:
    virtual size_t size() override;
    virtual RefStream refs() override;
    virtual void repr(std::ostream &out) override;
    virtual Object *copy_to(void *mem) override;
    virtual Object *call(size_t nargs, Object **args) override;
    virtual size_t num_slots() override;
    virtual bool takes_rest() override;
    static Builtin *make(size_t nargs, bool rest, fnative fnat);

  private:
    Builtin(size_t nargs, bool rest, fnative fnat);
    size_t nslots;
    bool has_rest;
    fnative *native;
};
} // namespace lithp

#endif // _LITHP_OBJECT_BUILTIN_H_
