#ifndef _LITHP_OBJECT_SPECIAL_FORM_H_
#define _LITHP_OBJECT_SPECIAL_FORM_H_

#include <lithp/object.hpp>
#include <lithp/object/list.hpp>
#include <lithp/object/symbol.hpp>

namespace lithp {
typedef Object *(*snative)(size_t, Object **, Environment &);

class SpecialForm : public Object {
    LITHP_NO_PRINT(SpecialForm);
    LITHP_NO_COPY(SpecialForm);

  public:
    virtual ~SpecialForm() = default;
    virtual Type type(void) override;
    virtual RefStream refs(void) override;
    static bool exists(Symbol *sym);
    static void init();
    static SpecialForm *cast(Object *obj);
    static bool is_instance(Object *obj);
    Object *eval(size_t nargs, Object **args, Environment &env);

  private:
    SpecialForm() = delete;
    SpecialForm(size_t nargs, bool rest, snative native);
    size_t nslots;
    bool has_rest;
    snative native;
};
} // namespace lithp

#endif // _LITHP_OBJECT_SPECIAL_FORM_H_
