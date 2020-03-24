#ifndef _LITHP_RUNTIME_HEAP_H_
#define _LITHP_RUNTIME_HEAP_H_

#include <cstdlib>

#define LITHP_HEAP_OBJECT(class_name)                                          \
  class_name() = delete;                                                       \
  class_name(const class_name &other) = delete;                                \
                                                                               \
public:                                                                        \
  virtual bool heap_allocated() override { return true; }                      \
  virtual size_t size() override { return sizeof(class_name); }

#define HEAP_NEW(class_name) new (allocator::get(sizeof(class_name))) class_name

namespace lithp::allocator {
// TODO: Should accept config about heap properties.
void init();
void shutdown();
void *get(size_t size);
} // namespace lithp::allocator

#endif // _LITHP_RUNTIME_HEAP_H_
