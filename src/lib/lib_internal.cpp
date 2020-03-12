#include <string>

#include "lib_io.hpp"
#include "lib_util.hpp"

namespace lithp::lib {
namespace internal {
Object *type_of(size_t, Object **args) {
  return SYM(type_name(lithp::type_of(args[0])));
}
} // namespace internal

void load_internal(Environment &env) {
  env.def(SYM("type-of"), FUN(1, false, internal::type_of));
}
} // namespace lithp::lib