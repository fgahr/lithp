#include "lib_numeric.hpp"
#include "lib_util.hpp"

namespace lithp::lib {

namespace numeric {
static Object *fadd(size_t nargs, Object **args) {
  int64_t result = 0;
  for (size_t i = 0; i < nargs; i++) {
    result += Number::cast(args[i])->int_value();
  }
  return Number::make(result);
}

static Object *fminus(size_t nargs, Object **args) {
  if (nargs == 0) {
    return Number::make(0);
  } else if (nargs == 1) {
    return Number::make(-AS_NUM(args[0])->int_value());
  } else {
    int64_t result = AS_NUM(args[0])->int_value();
    for (size_t i = 1; i < nargs; i++) {
      result -= AS_NUM(args[i])->int_value();
    }
    return Number::make(result);
  }
}

static Object *fmult(size_t nargs, Object **args) {
  int64_t result = 1;
  for (size_t i = 0; i < nargs; i++) {
    result *= Number::cast(args[i])->int_value();
  }
  return Number::make(result);
}

static Object *fdivide(size_t nargs, Object **args) {
  int64_t result = AS_NUM(args[0])->int_value() / AS_NUM(args[1])->int_value();
  return Number::make(result);
}

static Object *fis_number(size_t nargs, Object **args) {
  return Boolean::of(Number::is_instance(args[0]));
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
