#ifndef _LITHP_OBJECT_FUNCTION_H_
#define _LITHP_OBJECT_FUNCTION_H_

#include <vector>

#include <lithp/object.hpp>
#include <lithp/object/list.hpp>

namespace lithp {

using SlotArgs = std::vector<Object *>;
using RestArgs = List *;

class Function : public Object {
  public:
    virtual ~Function() = default;
    virtual Type type() override;
    virtual Object *call(size_t nargs, Object **args) = 0;
    virtual size_t num_slots() = 0;
    virtual bool takes_rest() = 0;
    static bool is_instance(Object *obj);
    static bool eq(Function *f1, Function *f2);
    static Function *cast(Object *);
};
} // namespace lithp

#endif // _LITHP_OBJECT_FUNCTION_H_
