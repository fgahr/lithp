#include <lithp.hpp>

namespace lithp {

runtime::Allocator *Number::allocator = nullptr;

Number *Number::make(long value) { return allocator->allocate_number(value); }

Number::Number(long value) : value{value} {}

Object *Number::eval(Environment &env) { return this; }

void Number::repr(std::ostream &out) { out << value; }

RefStream Number::refs() { return RefStream::empty(); }

Object *Number::copy_to(void *mem) { return new (mem) Number{this->value}; }

Number *Number::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Number); }

bool Number::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Number); }

bool Number::eq(Number *n1, Number *n2) { return n1->value == n2->value; }

Number *Number::add(std::vector<Number *> numbers) {
  long sum = 0;
  for (auto num : numbers) {
    sum += num->value;
  }
  return Number::make(sum);
}
} // namespace lithp
