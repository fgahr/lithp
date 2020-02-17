#ifndef _LITHP_RUNTIME_RUNTIME_H_
#define _LITHP_RUNTIME_RUNTIME_H_

#include <runtime/allocator.hpp>
#include <runtime/environment.hpp>
#include <util/refstream.hpp>

namespace lithp::runtime {
class Runtime {
public:
  void init();
  RefStream refs();
  Environment &base_env();

private:
  Allocator *alloc;
  Environment env;
};
} // namespace lithp::runtime

#endif // _LITHP_RUNTIME_RUNTIME_H_
