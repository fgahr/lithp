#include <vector>

#include <runtime/stack.hpp>

#define LITHP_STACK_SIZE 8192

namespace lithp::runtime::stack {
static Object *stack[LITHP_STACK_SIZE];
static std::vector<Ref> frames;
static size_t pos = 0;

static bool full() { return pos == LITHP_STACK_SIZE - 1; }
static Ref current_frame() { return frames.back(); }

Object *get(Ref ref) {
  if (ref > pos) {
    throw std::runtime_error{"illegal stack access"};
  }
  return stack[ref];
}

void set(Ref ref, Object *obj) {
  if (ref > pos) {
    throw std::runtime_error{"illegal stack access"};
  }
  stack[ref] = obj;
}

Object **ptr() { return &stack[pos]; }

Ref push(Object *obj) {
  if (full()) {
    throw std::runtime_error{"stack overflow"};
  }
  stack[pos] = obj;
  // Post-increment: first call should yield 0, etc.
  return pos++;
}

Object *pop() {
  if (pos == 0) {
    throw std::runtime_error{"stack underflow"};
  }
  return stack[pos--];
}

Ref new_frame(Object *code) {
  stack[pos] = code;
  frames.push_back(pos);
  // Post-increment: first call should yield 0, etc.
  return pos++;
}

static size_t num_args_in_frame() {
  Ref fpos = current_frame();
  if (pos <= fpos) {
    throw std::logic_error{"illegal stack state"};
  }

  Ref begin_args = fpos + 1;
  size_t nargs = pos - begin_args;
  return nargs;
}

static Object **begin_args_in_frame() {
  Ref apos = current_frame() + 1;
  if (apos >= LITHP_STACK_SIZE) {
    throw std::runtime_error{"stack overflow"};
  }
  return &stack[apos];
}

void call_in_current_frame(Function *fun) {
  size_t nargs = num_args_in_frame();
  Object **args = begin_args_in_frame();
  stack[current_frame()] = fun->call(nargs, args);
}

void eval_in_current_frame(SpecialForm *spec, Environment &env) {
  size_t nargs = num_args_in_frame();
  Object **args = begin_args_in_frame();
  stack[current_frame()] = spec->call(nargs, args, env);
}

Object *yield_frame() {
  pos = current_frame();
  Object *ret = stack[pos];
  frames.pop_back();
  return ret;
}

RefStream live_objects() {
  RefStream refs = RefStream::empty();
  for (Object *obj : stack) {
    RefStream orefs = obj->refs();
    // Append non-empty streams
    if (orefs.has_more()) {
      refs.append(std::move(orefs));
    }
  }

  // FIXME: Append stack references. Possibly rework GC entirely.

  return refs;
}

void reset() { pos = 0; }
} // namespace lithp::runtime::stack
