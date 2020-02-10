#include <refstream.hpp>

namespace lithp {
RefStream refs_of(std::vector<Object *> &objs) {
  std::vector<Object **> refs{objs.size()};
  size_t pos = 0;

  for (auto &o : objs) {
    refs[pos++] = &o;
  }

  return RefStream::of(refs);
}

RefStream refs_of(std::unordered_map<Symbol *, Object *> &pairs) {
  std::vector<Object **> refs{2 * pairs.size()};
  size_t pos = 0;

  for (auto &[key, value] : pairs) {
    refs[pos++] = (Object **)&key;
    refs[pos++] = &value;
  }

  return RefStream::of(refs);
}
} // namespace lithp
