#ifndef __LITHP_BROKEN_HEART_H_
#define __LITHP_BROKEN_HEART_H_

#include "object.hpp"

namespace lithp {
class BrokenHeart : public Object {
public:
  BrokenHeart(Object *redirect);
  virtual ~BrokenHeart() override = default;
  virtual size_t size() override { return redirect->size(); };
  virtual Type type() override { return Type::BrokenHeart; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  static bool is_instance(Object *obj);
  static BrokenHeart *cast(Object *obj);
  Object *const redirect;
};

} // namespace lithp

#endif // __LITHP_BROKEN_HEART_H_
