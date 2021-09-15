#include "lib_system.hpp"
#include "lib_util.hpp"

#include <lithp/runtime/heap.hpp>

namespace lithp::lib {
namespace system {
Object *run_gc(size_t, Object **) {
    allocator::run_gc();
    return nullptr;
}
} // namespace system

void load_system(Environment *env) {
    env->def(SYM("sys-run-gc"), FUN(0, false, system::run_gc));
}
} // namespace lithp::lib
