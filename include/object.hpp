#ifndef __LITHP_OBJECT_H_
#define __LITHP_OBJECT_H_

#include <ostream>

#include <util/refstream.hpp>

// TODO: Explore possibilities to use a class template for this
#define LITHP_HEAP_OBJECT(class_name)                                          \
  friend class Allocator;                                                      \
  class_name() = delete;                                                       \
  class_name(const class_name &other) = delete;                                \
                                                                               \
private:                                                                       \
  static Allocator *allocator;                                                 \
                                                                               \
public:                                                                        \
  static void init(Allocator &alloc) { allocator = &alloc; }                   \
  virtual bool heap_allocated() override { return true; }

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

class Allocator;
class Environment;

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

class Object;
class Environment;

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

#endif // __LITHP_OBJECT_H_
