#ifndef _LITHP_OBJECT_BOOLEAN_H_
#define _LITHP_OBJECT_BOOLEAN_H_

#include <object.hpp>

namespace lithp {
class Boolean : public Object {
  LITHP_NO_COPY(Boolean);

public:
  virtual ~Boolean() override = default;
  virtual size_t size() override { return sizeof(Boolean); }
  virtual Type type() override { return Type::Boolean; }
  virtual Object *evaluate(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  static Boolean *cast(Object *obj);
  static bool is_instance(Object *obj);
  static bool eq(Boolean *n1, Boolean *n2);
  static Boolean *True();
  static Boolean *False();
  static Boolean *of(bool val);

private:
  Boolean() = delete;
  Boolean(const Boolean &other) = delete;
  Boolean(bool value);
  bool value;
};

} // namespace lithp

#endif // _LITHP_OBJECT_BOOLEAN_H_
