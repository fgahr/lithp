#ifndef _LITHP_OBJECT_LAMBDA_H_
#define _LITHP_OBJECT_LAMBDA_H_

#include <object.hpp>
#include <object/function.hpp>
#include <object/symbol.hpp>
#include <runtime/environment.hpp>
#include <runtime/heap.hpp>

namespace lithp {
class Lambda : public Function {
  LITHP_HEAP_OBJECT(Lambda);

public:
  static Lambda *of(size_t nargs, Object **args, Environment &env);
  virtual ~Lambda() override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  virtual size_t num_slots() override;
  virtual bool takes_rest() override;
  virtual Object *call(size_t nargs, Object **args) override;
  static bool is_instance(Object *obj);
  static bool eq(Lambda *l1, Lambda *l2);

private:
  Lambda(size_t nargs, Symbol **syms, bool rest, Symbol *rest_sym, size_t progc,
         Object **progv, Environment &parent);
  size_t nslots;
  Symbol **slot_syms;
  bool has_rest;
  Symbol *rest_sym;
  size_t progc;
  Object **progv;
  Environment &parent;
};

} // namespace lithp

#endif // _LITHP_OBJECT_LAMBDA_H_
