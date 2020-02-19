#ifndef _LITHP_OBJECT_LIST_H_
#define _LITHP_OBJECT_LIST_H_

#include <object.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class List : public Object {
  LITHP_HEAP_OBJECT(List);

public:
  virtual ~List() override = default;
  virtual Type type() override { return Type::ConsCell; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  void set_car(Object *obj);
  void set_cdr(Object *obj);
  Object *car();
  Object *cdr();
  static bool is_instance(Object *obj);
  static List *cast(Object *obj);
  static List *make(Object *car, Object *cdr);
  static List *of(std::vector<Object *> objects);
  static size_t length(List *list);

private:
  List(Object *__car, Object *__cdr);
  Object *_car;
  Object *_cdr;
};

} // namespace lithp

#endif // _LITHP_OBJECT_LIST_H_
