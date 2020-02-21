#include "lib_numeric.hpp"
#include "lib_util.hpp"

namespace lithp::lib {

namespace numeric {
static std::vector<Number *> as_nums(List *list) {
  std::vector<Number *> nums;
  for (List *rest = list; rest; rest = List::cast(cdr(rest))) {
    nums.push_back(Number::cast(car(rest)));
  }
  return nums;
}

static Object *fadd(SlotArgs, RestArgs rest) {
  return Number::add(as_nums(rest));
}

static Object *fminus(SlotArgs, RestArgs rest) {
  return Number::minus(as_nums(rest));
}

static Object *fmult(SlotArgs, RestArgs rest) {
  return Number::mult(as_nums(rest));
}

static Object *fdivide(SlotArgs slots, RestArgs) {
  return Number::divide(AS_NUM(ARG0), AS_NUM(ARG1));
}

static Object *fis_number(SlotArgs slots, RestArgs) {
  return Boolean::of(Number::is_instance(ARG0));
}
} // namespace numeric

void load_numeric(Environment &env) {
  env.set(SYM("+"), FUN(0, true, numeric::fadd));
  env.set(SYM("-"), FUN(0, true, numeric::fminus));
  env.set(SYM("*"), FUN(0, true, numeric::fmult));
  env.set(SYM("/"), FUN(2, false, numeric::fdivide));
  env.set(SYM("number?"), FUN(1, false, numeric::fis_number));
}
} // namespace lithp::lib
