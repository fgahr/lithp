#ifndef _LITHP_OBJECT_BROKEN_HEART_H_
#define _LITHP_OBJECT_BROKEN_HEART_H_

#include <object.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class BrokenHeart : public Object {
  LITHP_HEAP_OBJECT(BrokenHeart);
  LITHP_NO_PRINT(BrokenHeart);
  LITHP_NO_COPY(BrokenHeart);

public:
  BrokenHeart(Object *redirect);
  virtual ~BrokenHeart() override = default;
  virtual Type type() override { return Type::BrokenHeart; }
  virtual RefStream refs() override;
  static bool is_instance(Object *obj);
  static BrokenHeart *cast(Object *obj);
  static bool eq(BrokenHeart *b1, BrokenHeart *b2);
  Object *const redirect;
};

} // namespace lithp

#endif // _LITHP_OBJECT_BROKEN_HEART_H_
