#ifndef _LITHP_LITHP_H_
#define _LITHP_LITHP_H_

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <object.hpp>
#include <object/boolean.hpp>
#include <object/broken_heart.hpp>
#include <object/builtin.hpp>
#include <object/function.hpp>
#include <object/lambda.hpp>
#include <object/list.hpp>
#include <object/number.hpp>
#include <object/special_form.hpp>
#include <object/symbol.hpp>
#include <runtime/environment.hpp>
#include <runtime/runtime.hpp>
#include <util/refstream.hpp>

namespace lithp {
Object *nil();
bool is_null(Object *obj);
bool is_pair(Object *obj);
Object *car(List *list);
Object *cdr(List *list);
Object *nth(size_t n, List *list);
bool eq(Object *o1, Object *o2);
Type type_of(Object *obj);
bool is_true(Object *obj);
bool is_false(Object *obj);

Object *eval(Object *obj, Environment &env);
Object *eval_sequence(List *seq, Environment &env);
Object *apply(Function *fun, List *args, Environment &env);
List *cons(Object *car, Object *cdr);
// TODO: Replace with something else?
std::string to_string(Object *obj);
} // namespace lithp

#endif // _LITHP_LITHP_H_
