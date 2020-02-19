#ifndef _LITHP_OBJECT_NIL_H_
#define _LITHP_OBJECT_NIL_H_

#include <object.hpp>

namespace lithp {
class Nil : public Object {
  LITHP_NO_COPY(Nil);

public:
  virtual ~Nil() = default;
  virtual size_t size() override;
  virtual Type type() override;
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  static bool eq(Nil *n1, Nil *n2);
  static Nil *nil();
  static Nil *cast(Object *obj);
  static bool is_instance(Object *obj);

private:
  Nil() = default;
  Nil(const Nil &other) = delete;
  static Nil instance;
};
} // namespace lithp

#endif // _LITHP_OBJECT_NIL_H_
