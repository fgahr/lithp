#include <lithp/lithp.hpp>

#include "lib_logic.hpp"
#include "lib_util.hpp"

namespace lithp::lib {
namespace logic {
Object *not_(size_t, Object **args) {
    if (is_true(args[0])) {
        return Boolean::False();
    } else {
        return Boolean::True();
    }
}

Object *true_(size_t, Object **args) {
    return Boolean::of(args[0] == Boolean::True());
}

Object *false_(size_t, Object **args) {
    return Boolean::of(args[0] == Boolean::False());
}
} // namespace logic
void load_logic(Environment *env) {
    env->def(SYM("not"), FUN(1, false, logic::not_));
    env->def(SYM("true?"), FUN(1, false, logic::true_));
    env->def(SYM("false?"), FUN(1, false, logic::false_));
}
} // namespace lithp::lib
