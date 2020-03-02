#include <stdexcept>

#include <object/broken_heart.hpp>

namespace lithp {
BrokenHeart::BrokenHeart(Object *redirect) : redirect{redirect} {}

RefStream BrokenHeart::refs() {
  throw std::logic_error{"attempting to get refs of a BrokenHeart"};
}

bool BrokenHeart::is_instance(Object *obj) {
  LITHP_CHECK_TYPE(obj, BrokenHeart);
}

BrokenHeart *BrokenHeart::cast(Object *obj) {
  LITHP_CAST_TO_TYPE(obj, BrokenHeart);
}

bool BrokenHeart::eq(BrokenHeart *b1, BrokenHeart *b2) {
  return b1->redirect == b2->redirect;
}
} // namespace lithp
