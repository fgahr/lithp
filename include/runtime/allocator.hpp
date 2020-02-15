#ifndef _LITHP_RUNTIME_ALLOCATOR_H_
#define _LITHP_RUNTIME_ALLOCATOR_H_

#include <cstdlib>
#include <types.hpp>

namespace lithp {

class StackFrame;

namespace runtime {
class Allocator {
public:
  Allocator(StackFrame *base_frame,
            size_t mem_size = 0x800000); // 8MB default
  Allocator() = delete;
  Allocator(const Allocator &alloc) = delete;
  ~Allocator();
  Number *allocate_number(long value);
  ConsCell *allocate_cons(Object *car, Object *cdr);

private:
  StackFrame *base_frame;
  char *heaps[2];
  size_t heap_pos = 0;
  size_t heap_idx = 0;
  size_t mem_size;
  void *heap_ptr();
  void ensure_space(size_t amount);
  void do_gc();
  void relocate(Object **obj, char *target, size_t *pos);
  void double_heap_size();
};

} // namespace runtime
} // namespace lithp

#endif // _LITHP_RUNTIME_ALLOCATOR_H_
