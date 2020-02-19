#ifndef _LITHP_RUNTIME_HEAP_H_
#define _LITHP_RUNTIME_HEAP_H_

#include <runtime/allocator.hpp>

#define LITHP_HEAP_OBJECT(class_name)                                          \
  class_name() = delete;                                                       \
  class_name(const class_name &other) = delete;                                \
                                                                               \
private:                                                                       \
  static runtime::Allocator *allocator;                                        \
                                                                               \
public:                                                                        \
  static void init(runtime::Allocator &alloc) { allocator = &alloc; }          \
  virtual bool heap_allocated() override { return true; }                      \
  virtual size_t size() override { return sizeof(class_name); }

#define HEAP_NEW(class_name) new (allocator->allocate(sizeof(class_name)))

#endif // _LITHP_RUNTIME_HEAP_H_
