#ifndef _LITHP_OBJECT_SPECIAL_FORM_H_
#define _LITHP_OBJECT_SPECIAL_FORM_H_

#include <object.hpp>
#include <object/list.hpp>
#include <object/symbol.hpp>

namespace lithp {
typedef Object *(*snative)(size_t, Object **, Environment &);

class SpecialForm {
public:
  static bool exists(Symbol *sym);
  static SpecialForm *get(Symbol *sym);
  Object *call(size_t nargs, Object **args, Environment &env);

private:
  static void init();
  SpecialForm(size_t nargs, bool rest, snative native);
  size_t nslots;
  bool has_rest;
  snative native;
};
} // namespace lithp

#endif // _LITHP_OBJECT_SPECIAL_FORM_H_
