#ifndef _LITHP_OBJECT_NUMBER_H_
#define _LITHP_OBJECT_NUMBER_H_

#include <vector>

#include <object.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class Number : public Object {
  LITHP_HEAP_OBJECT(Number);

public:
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
  static Number *add(std::vector<Number *> numbers);

private:
  Number(long value);
  long value;
};

} // namespace lithp

#endif // _LITHP_OBJECT_NUMBER_H_
