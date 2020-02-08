#ifndef __LITHP_LAMBDA_H_
#define __LITHP_LAMBDA_H_

#include <array>

#include "environment.hpp"
#include "object.hpp"
#include "symbol.hpp"

#define MAX_NUM_ARGS 8

namespace lithp {
using FnSlots = std::array<Object *, MAX_NUM_ARGS>;

class Lambda : public Object {
public:
  static Lambda *of(std::vector<Symbol *> args, Symbol *rest, Object *body);
  virtual ~Lambda() override;
  virtual size_t size() override { return sizeof(Lambda); }
  virtual Type type() override { return Type::Lambda; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  Object *call(std::vector<Object *> args);
  static Lambda *cast(Object *obj);
  static bool is_instance(Object *obj);

private:
  Lambda(size_t nargs, FnSlots &&slots);
  size_t num_args;
  FnSlots slots;
  Object *rest_args;
  Environment env;
  std::vector<Object *> program;
};

} // namespace lithp

#endif // __LITHP_LAMBDA_H_
