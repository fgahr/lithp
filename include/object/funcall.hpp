#ifndef _LITHP_OBJECT_FUNCALL_H_
#define _LITHP_OBJECT_FUNCALL_H_

#include <object.hpp>
#include <object/function.hpp>
#include <object/cons_cell.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class Funcall : public Object {
  LITHP_HEAP_OBJECT(Funcall);

public:
  virtual ~Funcall() override = default;
  virtual Type type() override;
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  static Funcall *make(Function *func, List *fargs);

private:
  Funcall(Function *func, List *args);
  Function *func;
  List *fargs;
};
} // namespace lithp

#endif // _LITHP_OBJECT_FUNCALL_H_
