#include <lib/stdlib.hpp>
#include <lithp.hpp>

namespace lithp::runtime {
void Runtime::init() {
  allocator::init(this);
  lib::load_stdlib(env);
}

RefStream Runtime::refs() {
  // TODO
  return RefStream::empty();
}

Environment &Runtime::base_env() { return env; }
} // namespace lithp::runtime
