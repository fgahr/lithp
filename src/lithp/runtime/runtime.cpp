#include <lib/lib.hpp>
#include <lithp.hpp>

namespace lithp::runtime {
void Runtime::init() {
  alloc = new Allocator{this};

  List::init(*alloc);
  Funcall::init(*alloc);
  Lambda::init(*alloc);
  Number::init(*alloc);

  lib::load_stdlib(env);
}

RefStream Runtime::refs() {
  // TODO
  return RefStream::empty();
}

Environment &Runtime::base_env() { return env; }
} // namespace lithp::runtime
