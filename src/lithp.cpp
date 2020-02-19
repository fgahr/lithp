#include <lithp.hpp>

namespace lithp {
Object *nil() { return nullptr; }
bool is_null(Object *obj) { return obj == nullptr; }
List *cons(Object *car, Object *cdr) { return ConsCell::make(car, cdr); }
} // namespace lithp
