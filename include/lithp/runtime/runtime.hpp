#ifndef _LITHP_RUNTIME_RUNTIME_H_
#define _LITHP_RUNTIME_RUNTIME_H_

#include <lithp/runtime/environment.hpp>
#include <lithp/util/refstream.hpp>

namespace lithp {
class Builtin;
namespace runtime {
void init();
RefStream live_objects();
Environment &global_env();
void register_builtin(Builtin *b);
void shutdown();
} // namespace runtime
} // namespace lithp

#endif // _LITHP_RUNTIME_RUNTIME_H_
