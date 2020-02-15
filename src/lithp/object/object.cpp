#include <object.hpp>
#include <object/broken_heart.hpp>
#include <object/cons_cell.hpp>
#include <object/lambda.hpp>
#include <object/number.hpp>
#include <object/symbol.hpp>

namespace lithp {

namespace {
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
  static Nil *cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Nil); }
  static bool eq(Nil *n1, Nil *n2) { return true; }
};
} // namespace

static Nil __nil;

Object *Object::nil() { return &__nil; }

bool Object::is_nil(Object *obj) { return obj == &__nil; }

bool Object::eq(Object *o1, Object *o2) {
  if (o1 == nullptr || o2 == nullptr) {
    throw std::logic_error{"attempting to compare NULL pointer"};
  }

  if (o1 == o2) {
    return true;
  }

  Type t = o1->type();
  if (t != o2->type()) {
    return false;
  }

#define EQ_ON_TYPE_CASE(t, o1, o2)                                             \
  case Type::t:                                                                \
    return t::eq(t::cast(o1), t::cast(o2));

  switch (t) {
    EQ_ON_TYPE_CASE(Nil, o1, o2);
    EQ_ON_TYPE_CASE(Number, o1, o2);
    EQ_ON_TYPE_CASE(Symbol, o1, o2);
    EQ_ON_TYPE_CASE(ConsCell, o1, o2);
    EQ_ON_TYPE_CASE(Function, o1, o2);
    EQ_ON_TYPE_CASE(BrokenHeart, o1, o2);
  default:
    return false;
  }

#undef EQ_ON_TYPE_CASE
}

} // namespace lithp
