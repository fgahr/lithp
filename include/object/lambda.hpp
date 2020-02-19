#ifndef _LITHP_OBJECT_LAMBDA_H_
#define _LITHP_OBJECT_LAMBDA_H_

#include <array>

#include <object.hpp>
#include <object/funcall.hpp>
#include <object/symbol.hpp>
#include <runtime/environment.hpp>
#include <runtime/heap.hpp>

#define MAX_NUM_ARGS 8

namespace lithp {
using FnSlots = std::array<Object *, MAX_NUM_ARGS>;

class Lambda : public Function {
  LITHP_HEAP_OBJECT(Lambda);

public:
  static Lambda *of(std::vector<Symbol *> args, Symbol *rest, Object *body);
  virtual ~Lambda() override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  virtual size_t num_args() override;
  virtual bool takes_rest() override;
  virtual Object *call(SlotArgs args, RestArgs rest) override;
  static bool is_instance(Object *obj);
  static bool eq(Lambda *l1, Lambda *l2);

private:
  Lambda(size_t nargs, FnSlots &&slots);
  size_t nargs;
  FnSlots slots;
  Object *rest_args;
  Environment env;
  std::vector<Object *> program;
};

} // namespace lithp

#endif // _LITHP_OBJECT_LAMBDA_H_
