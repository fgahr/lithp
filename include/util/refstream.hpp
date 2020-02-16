#ifndef _LITHP_UTIL_REFSTREAM_H_
#define _LITHP_UTIL_REFSTREAM_H_

#include <unordered_map>
#include <vector>

#include <util/stream.hpp>

namespace lithp {
class Object;
class Symbol;
using RefStream = util::Stream<Object **>;

RefStream refs_of(std::vector<Object *> &objs);
RefStream refs_of(std::unordered_map<Symbol *, Object *> &pairs);
} // namespace lithp

#endif // _LITHP_UTIL_REFSTREAM_H_
