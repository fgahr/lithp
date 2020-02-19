#ifndef _LITHP_OBJECT_CONS_CELL_H_
#define _LITHP_OBJECT_CONS_CELL_H_

#include <object/list.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class ConsCell : public List {
  LITHP_HEAP_OBJECT(ConsCell);

public:
  virtual ~ConsCell() override = default;
  virtual Type type() override { return Type::ConsCell; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  virtual Object *head() override;
  virtual Object *tail() override;
  void set_car(Object *obj);
  void set_cdr(Object *obj);
  Object *get_car();
  Object *get_cdr();
  static bool is_instance(Object *obj);
  static ConsCell *cast(Object *obj);
  static ConsCell *make(Object *car, Object *cdr);

private:
  ConsCell(Object *car, Object *cdr);
  Object *car;
  Object *cdr;
};

} // namespace lithp

#endif // _LITHP_OBJECT_CONS_CELL_H_
