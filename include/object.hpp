#ifndef _LITHP_OBJECT_H_
#define _LITHP_OBJECT_H_

#include <ostream>

#include <types.hpp>
#include <util/refstream.hpp>

#define LITHP_CAST_TO_TYPE(obj, t)                                             \
  if ((obj) == nullptr) {                                                      \
    throw std::runtime_error{"attempting to convert null pointer to type " +   \
                             type_name(Type::t)};                              \
  }                                                                            \
  if ((obj)->type() != Type::t) {                                              \
    throw std::logic_error{"illegal type conversion from " +                   \
                           type_name((obj)->type()) + " to " +                 \
                           type_name(Type::t)};                                \
  }                                                                            \
  return static_cast<t *>(obj)

#define LITHP_CHECK_TYPE(obj, t)                                               \
  if (obj != nullptr) {                                                        \
    return (obj)->type() == Type::t;                                           \
  }                                                                            \
  return false

namespace lithp {

namespace runtime {
class Environment;
}
using runtime::Environment;

class Object {
public:
  virtual bool heap_allocated() { return false; }
  virtual size_t size() = 0;
  virtual Type type() = 0;
  virtual Object *eval(Environment &env) = 0;
  virtual void repr(std::ostream &out) = 0;
  virtual RefStream refs() = 0;
  virtual Object *copy_to(void *mem) = 0;
  virtual ~Object() = default;
  static Object *nil();
  static bool is_nil(Object *obj);
  static bool eq(Object *o1, Object *o2);
};

} // namespace lithp

#endif // _LITHP_OBJECT_H_
