#include <cstdlib>
#include <stdexcept>

#include <object/broken_heart.hpp>
#include <runtime/allocator.hpp>
#include <runtime/runtime.hpp>

namespace lithp::runtime {
Allocator::Allocator(Runtime *runtime, size_t mem_size)
    : runtime{runtime}, mem_size{mem_size} {
  heaps[0] = (char *)std::calloc(mem_size, sizeof(char));
  heaps[1] = (char *)std::calloc(mem_size, sizeof(char));

  if (!heaps[0] || !heaps[1]) {
    throw std::runtime_error{"memory allocation failed"};
  }
}

Allocator::~Allocator() {
  if (heaps[0]) {
    free(heaps[0]);
  }
  if (heaps[1]) {
    free(heaps[1]);
  }
}

void *Allocator::allocate(size_t size) {
  ensure_space(size);
  void *allocated = heap_ptr();
  heap_pos += size;
  return allocated;
}

void *Allocator::heap_ptr() { return &heaps[heap_idx][heap_pos]; }

void Allocator::ensure_space(size_t amount) {
  if (heap_pos + amount >= mem_size) {
    do_gc();
    if (heap_pos > 0.8 * mem_size) { // heap more than 80% full
      double_heap_size();
    }
  }
}

void Allocator::do_gc() {
  RefStream refs = runtime->refs();
  char *new_heap = heaps[!heap_idx];
  size_t pos = 0;

  while (refs.has_more()) {
    relocate(refs.get(), new_heap, &pos);
  }

  // Call destructors for obsolete objects
  size_t size;
  while (pos < mem_size) {
    Object *obj = (Object *)&heaps[heap_idx][pos];
    size = obj->size();
    delete obj;
    pos += size;
  }

  // Use new heap
  heap_idx = !heap_idx;
}

void Allocator::relocate(Object **ref, char *target, size_t *pos) {
  if (ref == nullptr || *ref == nullptr) {
    throw std::runtime_error{"null pointer encounter during relocation"};
  }

  if (!(*ref)->heap_allocated()) {
    // Nothing to do
    return;
  }

  if (BrokenHeart::is_instance(*ref)) {
    // Already moved, adjust pointer
    *ref = BrokenHeart::cast(*ref)->redirect;
    return;
  }

  Object *moved = (*ref)->copy_to(&target[*pos]);
  void *old_loc = *ref;
  delete *ref;
  new (old_loc) BrokenHeart{moved};
  *ref = moved;
  *pos += moved->size();
}

void Allocator::double_heap_size() {
  mem_size *= 2;
  heaps[0] = (char *)realloc(heaps[0], mem_size);

  if (!heaps[0] || !heaps[1]) {
    throw std::runtime_error{"memory allocation failed"};
  }
}
} // namespace lithp::runtime
