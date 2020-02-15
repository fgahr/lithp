#ifndef __LITHP_FUNCALL_H_
#define __LITHP_FUNCALL_H_

#include <object.hpp>
#include <object/function.hpp>

namespace lithp {
class Funcall : public Object {
  LITHP_HEAP_OBJECT(Funcall);

public:
  virtual ~Funcall() override;
  virtual size_t size() override;
  virtual Type type() override;
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;

private:
  Function *func;
  std::vector<Object *> fargs;
};
} // namespace lithp

#endif // __LITHP_FUNCALL_H_
