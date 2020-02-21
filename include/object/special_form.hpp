#ifndef _LITHP_OBJECT_SPECIAL_FORM_H_
#define _LITHP_OBJECT_SPECIAL_FORM_H_

#include <object.hpp>
#include <object/list.hpp>
#include <object/symbol.hpp>

namespace lithp::special {
bool is_special(Symbol *sym);
Object *dispatch(Symbol *type, List *args, Environment &env);
} // namespace lithp::special

#endif // _LITHP_OBJECT_SPECIAL_FORM_H_
