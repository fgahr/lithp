#include <object.hpp>
#include <util/refstream.hpp>

namespace lithp {
RefStream refs_of(std::vector<Object *> &objs) {
  std::vector<RefStream> streams;
  for (auto &obj : objs) {
    streams.push_back(RefStream::concat(RefStream::of({&obj}), obj->refs()));
  }

  RefStream refs = RefStream::empty();
  for (auto &s : streams) {
    refs.append(std::move(s));
  }
  return refs;
}

RefStream refs_of(std::unordered_map<Symbol *, Object *> &pairs) {
  std::vector<RefStream> streams;
  for (auto &[key, val] : pairs) {
    streams.push_back(RefStream::concat(RefStream::of({&val}), val->refs()));
  }

  RefStream refs = RefStream::empty();
  for (auto &s : streams) {
    refs.append(std::move(s));
  }
  return refs;
}
} // namespace lithp
