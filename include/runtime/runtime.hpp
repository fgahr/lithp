#ifndef _LITHP_RUNTIME_RUNTIME_H_
#define _LITHP_RUNTIME_RUNTIME_H_

#include <runtime/environment.hpp>
#include <runtime/heap.hpp>
#include <util/refstream.hpp>

namespace lithp::runtime {
class Runtime {
public:
  ~Runtime();
  void init();
  RefStream refs();
  Environment &base_env();
  void read(std::istream &in);

private:
  Environment env;
};
} // namespace lithp::runtime

#endif // _LITHP_RUNTIME_RUNTIME_H_
