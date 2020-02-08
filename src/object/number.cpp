#include "number.hpp"

namespace lithp {
Number::Number(long value) : value{value} {}

Object *Number::eval(Environment &env) { return this; }

void Number::repr(std::ostream &out) { out << value; }

RefStream Number::refs() { return RefStream::empty(); }

Object *Number::copy_to(void *mem) { return new (mem) Number{this->value}; }

Number *Number::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Number); }

bool Number::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Number); }
} // namespace lithp