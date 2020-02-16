#ifndef _LITHP_OBJECT_TYPES_H_
#define _LITHP_OBJECT_TYPES_H_

#include <string>

namespace lithp {

class Object;
class Number;
class Boolean;
class Symbol;
class ConsCell;
class Reference;
class Function;
class Funcall;
class BrokenHeart;

enum class Type {
  Nil,
  Number,
  Boolean,
  Symbol,
  ConsCell,
  Reference,
  Function,
  Funcall,
  BrokenHeart,
};

std::string type_name(Type t);
} // namespace lithp

#endif // _LITHP_OBJECT_TYPES_H_
