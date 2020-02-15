#include <lithp.hpp>

namespace lithp::runtime {
void Runtime::init() {
  ConsCell::init(allocator);
  Funcall::init(allocator);
  Lambda::init(allocator);
  Number::init(allocator);
}

RefStream Runtime::refs() {
  // TODO
  return RefStream::empty();
}
} // namespace lithp::runtime
