#ifndef _LITHP_OBJECT_SYMBOL_H_
#define _LITHP_OBJECT_SYMBOL_H_

#include <string_view>

#include <lithp/object.hpp>

namespace lithp {

class SymbolChain;

class Symbol : public Object {
  friend class SymbolChain;

  LITHP_NO_COPY(Symbol);

public:
  virtual ~Symbol() override = default;
  virtual Type type() override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  static bool is_instance(Object *obj);
  static Symbol *cast(Object *obj);
  static bool eq(Symbol *s1, Symbol *s2);
  static Symbol *intern(std::string name);
  static bool is_valid(std::string_view name);
  bool self_evaluating();
  std::string get_name();

private:
  Symbol() = delete;
  Symbol(const Symbol &other) = delete;
  Symbol(std::string name);
  std::string name;
};
} // namespace lithp

#endif // _LITHP_OBJECT_SYMBOL_H_
