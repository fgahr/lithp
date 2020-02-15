#ifndef __LITHP_TYPES_H_
#define __LITHP_TYPES_H_

#include <string>

namespace lithp {

class Object;
class Number;
class Boolean;
class Symbol;
class ConsCell;
class Reference;
class Function;
class BrokenHeart;

enum class Type {
  Nil,
  Number,
  Boolean,
  Symbol,
  ConsCell,
  Reference,
  Function,
  BrokenHeart,
};

std::string type_name(Type t);
} // namespace lithp

#endif // __LITHP_TYPES_H_
