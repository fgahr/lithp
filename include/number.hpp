#ifndef __LITHP_NUMBER_H_
#define __LITHP_NUMBER_H_

#include <object.hpp>

namespace lithp {
class Number : public Object {
public:
  Number(long value);
  virtual ~Number() override = default;
  virtual size_t size() override { return sizeof(Number); }
  virtual Type type() override { return Type::Number; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  static Number *cast(Object *obj);
  static bool is_instance(Object *obj);
  static bool eq(Number *n1, Number *n2);

private:
  long value;
};

} // namespace lithp

#endif // __LITHP_NUMBER_H_
