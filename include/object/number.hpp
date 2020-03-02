#ifndef _LITHP_OBJECT_NUMBER_H_
#define _LITHP_OBJECT_NUMBER_H_

#include <vector>

#include <object.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class Number : public Object {
public:
  Number() = delete;
  Number(const Number &other) = delete;
  static Number *make(long value);
  virtual ~Number() override = default;
  virtual bool heap_allocated() override;
  virtual size_t size() override;
  virtual Type type() override { return Type::Number; }
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  long int_value();
  static Number *cast(Object *obj);
  static bool is_instance(Object *obj);
  static bool eq(Number *n1, Number *n2);
  static Number *add(std::vector<Number *> numbers);
  static Number *minus(std::vector<Number *> numbers);
  static Number *mult(std::vector<Number *> numbers);
  static Number *divide(Number *numerator, Number *denominator);

private:
  static std::array<Number, 257> small_numbers;
  Number(long value);
  long value;
};

} // namespace lithp

#endif // _LITHP_OBJECT_NUMBER_H_
