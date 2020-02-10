#ifndef __LITHP_FUNCALL_H_
#define __LITHP_FUNCALL_H_

#include <vector>

#include <object.hpp>

namespace lithp {
class Function : public Object {
public:
  virtual ~Function() = default;
  virtual Object *call(std::vector<Object *> args) = 0;
  static bool is_instance(Object *obj);
  static bool eq(Function *o1, Function *o2);
};

class Funcall : public Object {
  virtual ~Funcall() override;
  virtual size_t size() override;
  virtual Type type() override;
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;

private:
  Function *func;
  std::vector<Object *> args;
};
} // namespace lithp

#endif // __LITHP_FUNCALL_H_
