#ifndef _LITHP_OBJECT_H_
#define _LITHP_OBJECT_H_

#include <ostream>

#include <object/types.hpp>
#include <util/refstream.hpp>

#define LITHP_CAST_TO_TYPE(obj, t)                                             \
  if ((obj) == nullptr) {                                                      \
    throw std::runtime_error{"attempting to convert null to type " +           \
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

#define LITHP_NO_PRINT(class_name)                                             \
public:                                                                        \
  virtual void repr(std::ostream &) override {                                 \
    throw std::runtime_error{                                                  \
        "cannot print an instance of class " #class_name};                     \
  }

#define LITHP_NO_COPY(class_name)                                              \
public:                                                                        \
  virtual Object *copy_to(void *) override {                                   \
    throw std::logic_error{"cannot copy an instance of class " #class_name};   \
  }

namespace runtime {
class Environment;
}
using runtime::Environment;

class Object {
public:
  virtual bool heap_allocated(void) { return false; }
  virtual size_t size(void) { return 0; }
  virtual Type type(void) = 0;
  virtual void repr(std::ostream &out) = 0;
  virtual RefStream refs(void) = 0;
  virtual Object *copy_to(void *mem) = 0;
  virtual ~Object() = default;
};

} // namespace lithp

#endif // _LITHP_OBJECT_H_
