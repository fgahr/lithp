#ifndef _LITHP_OBJECT_CONS_CELL_H_
#define _LITHP_OBJECT_CONS_CELL_H_

#include <object.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class List : public Object {
  LITHP_HEAP_OBJECT(List);

public:
  virtual ~List() override = default;
  virtual Type type() override { return Type::List; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  Object *car;
  Object *cdr;
  static bool is_instance(Object *obj);
  static List *cast(Object *obj);
  static List *make(Object *car, Object *cdr);

private:
  List(Object *car, Object *cdr);
};

} // namespace lithp

#endif // _LITHP_OBJECT_CONS_CELL_H_
