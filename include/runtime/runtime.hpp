#ifndef _LITHP_RUNTIME_RUNTIME_H_
#define _LITHP_RUNTIME_RUNTIME_H_

#include <runtime/environment.hpp>
#include <util/refstream.hpp>

namespace lithp::runtime {
void init();
RefStream live_objects();
Environment &global_env();
void shutdown();
} // namespace lithp::runtime

#endif // _LITHP_RUNTIME_RUNTIME_H_
