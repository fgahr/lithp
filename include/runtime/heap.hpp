#ifndef _LITHP_RUNTIME_HEAP_H_
#define _LITHP_RUNTIME_HEAP_H_

#include <runtime/allocator.hpp>

#define LITHP_HEAP_OBJECT(class_name)                                          \
  friend class runtime::Allocator;                                             \
  class_name() = delete;                                                       \
  class_name(const class_name &other) = delete;                                \
                                                                               \
private:                                                                       \
  static runtime::Allocator *allocator;                                        \
                                                                               \
public:                                                                        \
  static void init(runtime::Allocator &alloc) { allocator = &alloc; }          \
  virtual bool heap_allocated() override { return true; }

#endif // _LITHP_RUNTIME_HEAP_H_
