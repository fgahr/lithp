#ifndef _LITHP_OBJECT_FUNCTION_H_
#define _LITHP_OBJECT_FUNCTION_H_

#include <array>
#include <vector>

#include <object.hpp>

namespace lithp {
using FnArgs = std::array<Object *, 8>;
using RestArgs = std::vector<Object *>;

class Function : public Object {
public:
  virtual ~Function() = default;
  virtual Type type() override;
  virtual Object *eval(Environment &env) override;
  virtual Object *call(FnArgs args, RestArgs rest) = 0;
  virtual size_t num_args() = 0;
  virtual bool takes_rest() = 0;
  static bool is_instance(Object *obj);
  static bool eq(Function *f1, Function *f2);
  static Function *cast(Object *);
};
} // namespace lithp

#endif // _LITHP_OBJECT_FUNCTION_H_
