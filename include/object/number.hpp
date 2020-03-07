#ifndef _LITHP_OBJECT_NUMBER_H_
#define _LITHP_OBJECT_NUMBER_H_

#include <vector>
#include <cstdint>

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
  int64_t int_value();
  static Number *cast(Object *obj);
  static bool is_instance(Object *obj);
  static bool eq(Number *n1, Number *n2);

private:
  static std::array<Number, 255> small_numbers;
  Number(int64_t value);
  int64_t value;
};

} // namespace lithp

#endif // _LITHP_OBJECT_NUMBER_H_
