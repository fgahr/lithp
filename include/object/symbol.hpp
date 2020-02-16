#ifndef _LITHP_OBJECT_SYMBOL_H_
#define _LITHP_OBJECT_SYMBOL_H_

#include <string_view>

#include <object.hpp>

namespace lithp {

class SymbolChain;

class Symbol : public Object {
  friend class SymbolChain;

public:
  virtual ~Symbol() override = default;
  virtual Type type() override;
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  static bool is_instance(Object *obj);
  static Symbol *cast(Object *obj);
  static bool eq(Symbol *s1, Symbol *s2);
  static Symbol *intern(std::string name);
  bool self_evaluating();
  std::string get_name();

private:
  Symbol() = delete;
  Symbol(const Symbol &other) = delete;
  Symbol(std::string name);
  std::string name;
  static bool is_valid(std::string_view name);
};
} // namespace lithp

#endif // _LITHP_OBJECT_SYMBOL_H_
