#include <stdexcept>

#include "broken_heart.hpp"

namespace lithp {
BrokenHeart::BrokenHeart(Object *redirect) : redirect{redirect} {}

Object *BrokenHeart::eval(Environment &env) {
  throw std::logic_error{"attempting to evaluate a BrokenHeart"};
}

void BrokenHeart::repr(std::ostream &out) {
  throw std::logic_error{"attempting to print a BrokenHeart"};
}

RefStream BrokenHeart::refs() {
  throw std::logic_error{"attempting to get refs of a BrokenHeart"};
}

Object *BrokenHeart::copy_to(void *mem) {
  throw std::logic_error{"attempting to copy a BrokenHeart"};
}

bool BrokenHeart::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, BrokenHeart); }

BrokenHeart *BrokenHeart::cast(Object *obj) {
  LITHP_CAST_TO_TYPE(obj, BrokenHeart);
}
} // namespace lithp
