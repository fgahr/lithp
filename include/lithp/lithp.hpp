#ifndef _LITHP_LITHP_H_
#define _LITHP_LITHP_H_

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <lithp/object.hpp>
#include <lithp/object/boolean.hpp>
#include <lithp/object/broken_heart.hpp>
#include <lithp/object/builtin.hpp>
#include <lithp/object/function.hpp>
#include <lithp/object/lambda.hpp>
#include <lithp/object/list.hpp>
#include <lithp/object/number.hpp>
#include <lithp/object/special_form.hpp>
#include <lithp/object/string.hpp>
#include <lithp/object/symbol.hpp>
#include <lithp/runtime/environment.hpp>
#include <lithp/runtime/runtime.hpp>
#include <lithp/runtime/stack.hpp>
#include <lithp/util/refstream.hpp>

namespace lithp {
Object *nil();
bool is_null(Object *obj);
bool is_pair(Object *obj);
List *cons(Object *car, Object *cdr);
Object *car(List *list);
Object *cdr(List *list);
Object *nth(size_t n, List *list);
bool eq(Object *o1, Object *o2);
Type type_of(Object *obj);
Type type_of(Object *obj);
bool is_true(Object *obj);
bool is_false(Object *obj);
bool is_definition(Object *obj);

Object *eval(Object *obj, Environment &env);
Object *eval_sequence(size_t n, Object **seq, Environment &env);
Object *apply(Function *fun, List *args, Environment &env);

std::string to_string(Object *obj);
std::string display(Object *obj);
} // namespace lithp

#endif // _LITHP_LITHP_H_
