#include "object.hpp"

namespace lithp {
#define TYPE_NAME_CASE(t)                                                      \
  case Type::t:                                                                \
    return #t

std::string type_name(Type t) {
  switch (t) {
    TYPE_NAME_CASE(Nil);
    TYPE_NAME_CASE(Number);
    TYPE_NAME_CASE(Symbol);
    TYPE_NAME_CASE(ConsCell);
    TYPE_NAME_CASE(Reference);
    TYPE_NAME_CASE(Lambda);
    TYPE_NAME_CASE(BrokenHeart);
  default:
    throw std::logic_error{"unknown type " +
                           std::to_string(static_cast<int>(t))};
  }
}

#undef TYPE_NAME_CASE

bool Object::has_type(Type t) { return type() == t; }

class Nil : public Object {
public:
  virtual size_t size() { return 0; }
  virtual Type type() { return Type::Nil; }
  virtual Object *eval(Environment &env) { return this; }
  virtual void repr(std::ostream &out) { out << "nil"; }
  virtual RefStream refs() { return RefStream::empty(); }
  virtual Object *copy_to(void *mem) {
    throw std::logic_error{"attempting to move nil"};
  }
  virtual ~Nil() = default;
};

static Nil __nil;

bool Object::is_nil(Object *obj) { return obj == &__nil; }

Object *Object::nil() { return &__nil; }

} // namespace lithp
