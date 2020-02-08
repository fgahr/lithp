#ifndef __LITHP_OBJECT_H_
#define __LITHP_OBJECT_H_

#include <ostream>

#include "stream.hpp"
#include "environment.hpp"

#define LITHP_CAST_TO_TYPE(obj, t)                                             \
  if ((obj) == nullptr) {                                                      \
    throw std::runtime_error{"attempting to convert null pointer to type " +   \
                             type_name(t)};                                    \
  }                                                                            \
  if (!(obj)->has_type(Type::t)) {                                             \
    throw std::logic_error{"illegal type conversion from " +                   \
                           type_name(type()) + " to " + type_name(Type::t)};   \
  }                                                                            \
  return static_cast<t *>(obj)

#define LITHP_CHECK_TYPE(obj, t)                                               \
  if (obj != nullptr) {                                                        \
    return (obj)->type() == Type::t;                                           \
  }                                                                            \
  return false

namespace lithp {

enum class Type {
  Nil,
  Number,
  Symbol,
  ConsCell,
  Reference,
  Lambda,
  BrokenHeart,
};

std::string type_name(Type t);

class Object;
class Environment;
using RefStream = util::Stream<Object **>;

class Object {
public:
  virtual size_t size() = 0;
  virtual Type type() = 0;
  virtual Object *eval(Environment &env) = 0;
  virtual void repr(std::ostream &out) = 0;
  virtual RefStream refs() = 0;
  virtual Object *copy_to(void *mem) = 0;
  virtual ~Object() = default;
  bool has_type(Type t);
  static bool is_nil(Object *obj);
  static Object *nil();
};

} // namespace lithp

#endif // __LITHP_OBJECT_H_
