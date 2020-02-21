#include <lib/stdlib.hpp>
#include <lithp.hpp>

namespace lithp::runtime {

Runtime::~Runtime() { allocator::shutdown(); }

void Runtime::init() {
  allocator::init(this);
  lib::load_stdlib(env);
  env.set(Symbol::intern("true"), Boolean::True());
  env.set(Symbol::intern("false"), Boolean::False());
}

RefStream Runtime::refs() {
  // TODO
  return RefStream::empty();
}

Environment &Runtime::base_env() { return env; }
} // namespace lithp::runtime
