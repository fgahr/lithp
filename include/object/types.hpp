#ifndef _LITHP_OBJECT_TYPES_H_
#define _LITHP_OBJECT_TYPES_H_

#include <string>

namespace lithp {

enum class Type {
  Boolean,
  BrokenHeart,
  ConsCell,
  Funcall,
  Function,
  Nil,
  Number,
  Placeholder,
  Reference,
  SpecialForm,
  Symbol,
};

std::string type_name(Type t);
} // namespace lithp

#endif // _LITHP_OBJECT_TYPES_H_
