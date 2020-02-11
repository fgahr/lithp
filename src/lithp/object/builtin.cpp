#include <object/builtin.hpp>

namespace lithp {
Object *Builtin::call(std::vector<Object *> args, Environment &env) {
  return nat(args, env);
}
} // namespace lithp
