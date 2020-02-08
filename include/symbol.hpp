#ifndef __LITHP_SYMBOL_H_
#define __LITHP_SYMBOL_H_

#include <string_view>

#include "object.hpp"

namespace lithp {
class Symbol : public Object {
public:
  Symbol(std::string name);
  virtual ~Symbol() override = default;
  virtual size_t size() override { return sizeof(Symbol); }
  virtual Type type() override { return Type::Symbol; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  static bool is_instance(Object *obj);
  static Symbol *cast(Object *obj);
  bool self_evaluating();
  const char *name;

private:
  Symbol(const char *name);
  static bool is_valid(std::string_view name);
  static const char *intern(std::string name);
};
} // namespace lithp

#endif // __LITHP_SYMBOL_H_
