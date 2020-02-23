#include <lithp.hpp>

namespace lithp {

Number *Number::make(long value) { return HEAP_NEW(Number){value}; }

Number::Number(long value) : value{value} {}

Object *Number::evaluate(Environment &) { return this; }

void Number::repr(std::ostream &out) { out << value; }

RefStream Number::refs() { return RefStream::empty(); }

Object *Number::copy_to(void *mem) { return new (mem) Number{this->value}; }

long Number::int_value() { return value; }
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

Number *Number::minus(std::vector<Number *> numbers) {
  switch (numbers.size()) {
  case 0:
    return Number::make(0);
  case 1:
    return Number::make(-numbers.front()->value);
  default:
    long diff = numbers.front()->value;
    for (size_t i = 1; i < numbers.size(); i++) {
      diff -= numbers.at(i)->value;
    }
    return Number::make(diff);
  }
}

Number *Number::mult(std::vector<Number *> numbers) {
  long product = 0;
  for (auto num : numbers) {
    product *= num->value;
  }
  return Number::make(product);
}

Number *Number::divide(Number *numerator, Number *denominator) {
  if (denominator->value == 0) {
    throw std::runtime_error{"attempted division by 0"};
  }
  return Number::make(numerator->value / denominator->value);
}
} // namespace lithp
