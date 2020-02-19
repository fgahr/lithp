#ifndef _LITHP_OBJECT_SPECIAL_FORM_H_
#define _LITHP_OBJECT_SPECIAL_FORM_H_

#include <unordered_map>
#include <vector>

#include <object.hpp>
#include <object/symbol.hpp>
#include <runtime/heap.hpp>

namespace lithp {

class SpecialForm;

typedef SpecialForm *(*builder)(std::vector<Object *>);

class SpecialForm : public Object {
public:
  virtual Type type() override;
  virtual ~SpecialForm();
  static bool is_instance(Object *obj);
  static bool is_special(Symbol *sym);
  static SpecialForm *dispatch(Symbol *type, std::vector<Object *> args);
  static void init(runtime::Allocator &alloc);

private:
  static std::unordered_map<Symbol *, builder> form_builders;
};
} // namespace lithp

#endif // _LITHP_OBJECT_SPECIAL_FORM_H_
