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
#include <object/funcall.hpp>
#include <object/function.hpp>
#include <object/lambda.hpp>
#include <object/list.hpp>
#include <object/number.hpp>
#include <object/special_form.hpp>
#include <object/symbol.hpp>
#include <runtime/allocator.hpp>
#include <runtime/environment.hpp>
#include <runtime/runtime.hpp>
#include <util/refstream.hpp>

namespace lithp {
Object *nil();
bool is_null(Object *);
List *cons(Object *car, Object *cdr);
} // namespace lithp

#endif // _LITHP_LITHP_H_
