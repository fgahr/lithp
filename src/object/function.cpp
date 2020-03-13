#include <object/function.hpp>

namespace lithp {

Type Function::type() { return Type::Function; }
bool Function::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Function); }
Function *Function::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Function); }
bool Function::eq(Function *f1, Function *f2) { return f1 == f2; }

} // namespace lithp
