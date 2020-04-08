#ifndef _LITHP_OBJECT_TYPES_H_
#define _LITHP_OBJECT_TYPES_H_

#include <string>

namespace lithp {

enum class Type {
  Boolean,
  BrokenHeart,
  Function,
  List,
  Nil,
  Number,
  String,
  Symbol,
};

std::string type_name(Type t);
} // namespace lithp

#endif // _LITHP_OBJECT_TYPES_H_
