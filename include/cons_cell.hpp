#ifndef __LITHP_CONS_CELL_H_
#define __LITHP_CONS_CELL_H_

#include "object.hpp"

namespace lithp {
class ConsCell : public Object {
public:
  ConsCell(Object *car, Object *cdr);
  virtual ~ConsCell() override = default;
  virtual size_t size() override { return sizeof(ConsCell); }
  virtual Type type() override { return Type::ConsCell; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  Object *car;
  Object *cdr;
  static bool is_instance(Object *obj);
  static ConsCell *cast(Object *obj);
};

} // namespace lithp

#endif // __LITHP_CONS_CELL_H_
