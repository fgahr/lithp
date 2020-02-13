#include <object/funcall.hpp>

namespace lithp {

Type Function::type() { return Type::Function; }

Object *Function::eval(Environment &env) {
  // NOTE: evaluating a function (say, as part of an argument list) is not the
  // same as calling it.
  return this;
}

Function *Function::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Function); }

bool Function::eq(Function *f1, Function *f2) { return f1 == f2; }

Object *Funcall::eval(Environment &env) { return func->call(args); }

} // namespace lithp
