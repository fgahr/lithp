#ifndef __LITHP_BUILTIN_H_
#define __LITHP_BUILTIN_H_

#include <funcall.hpp>

namespace lithp {
typedef Object *(native)(std::vector<Object *> args, Environment &env);
class Builtin : public Function {
  virtual Object *call(std::vector<Object *> args, Environment &env);

private:
  native *nat;
};
} // namespace lithp

#endif // __LITHP_BUILTIN_H_
