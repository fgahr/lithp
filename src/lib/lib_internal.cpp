#include <string>

#include <lithp/util/strings.hpp>

#include "lib_io.hpp"
#include "lib_util.hpp"

namespace lithp::lib {
namespace internal {
Object *type_of(size_t, Object **args) {
    Type type = lithp::type_of(args[0]);
    switch (type) {
    case Type::SpecialForm:
        return Symbol::intern("special-form");
    default:
        return SYM(util::str::downcase(type_name(lithp::type_of(args[0]))));
    }
}
} // namespace internal

void load_internal(Environment &env) {
    env.def(SYM("type-of"), FUN(1, false, internal::type_of));
}
} // namespace lithp::lib
