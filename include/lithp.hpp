#ifndef LITHP_LITHP_HPP
#define LITHP_LITHP_HPP

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <environment.hpp>
#include <object.hpp>
#include <object/broken_heart.hpp>
#include <object/builtin.hpp>
#include <object/cons_cell.hpp>
#include <object/funcall.hpp>
#include <object/function.hpp>
#include <object/lambda.hpp>
#include <object/number.hpp>
#include <object/symbol.hpp>
#include <util/refstream.hpp>

namespace lithp {

void print(Object *obj, std::ostream &out);

class StackFrame {
public:
  StackFrame(StackFrame *parent, Environment env,
             std::vector<Object *> instructions);
  StackFrame() = delete;
  StackFrame(const StackFrame &frame) = delete;
  ~StackFrame() = default;
  Object *run();
  RefStream refs();
  Environment &environment();

private:
  StackFrame *parent;
  StackFrame *child = nullptr;
  Environment env;
  std::vector<Object *> program;
};

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

class Interpreter {
public:
  void init();

private:
  Allocator allocator;
};

} // namespace lithp

#endif /* LITHP_LITHP_HPP */
