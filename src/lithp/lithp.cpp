#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <lithp.hpp>

namespace lithp {

StackFrame::StackFrame(StackFrame *parent, Environment env,
                       std::vector<Object *> instructions)
    : parent{parent}, env{std::move(env)}, program{instructions} {
  if (parent) {
    env = Environment{parent->env};
  } else {
    env = Environment{nullptr};
  }
}

Object *StackFrame::run() {
  Object *ret;
  for (Object *obj : program) {
    ret = obj->eval(env);
  }
  return ret;
}

RefStream StackFrame::refs() {
  RefStream child_refs = child ? child->refs() : RefStream::empty();
  return RefStream::concat(RefStream::concat(env.refs(), refs_of(program)),
                           std::move(child_refs));
}

Environment &StackFrame::environment() { return env; }

Allocator::Allocator(StackFrame *base_frame, size_t mem_size)
    : base_frame{base_frame}, mem_size{mem_size} {
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

Number *Allocator::allocate_number(long value) {
  const size_t size = sizeof(Number);
  ensure_space(size);
  auto num = new (heap_ptr()) Number{value};
  heap_pos += size;
  return num;
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
  RefStream refs = base_frame->refs();
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

  if (BrokenHeart::is_instance(*ref)) {
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

} // namespace lithp
