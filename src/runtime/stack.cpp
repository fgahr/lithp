#include <iostream>
#include <vector>

#include <lithp/runtime/stack.hpp>

#define LITHP_STACK_SIZE 8192

namespace lithp::runtime::stack {
/**
 * The stack, will only hold pointers to objects that are in the heap or
 * otherwise allocated (e.g. the symbol table).
 *
 * NOTE: Could be made resizable.
 */
static Object *stack[LITHP_STACK_SIZE];
/**
 * Active frame pointers. Note that we cannot store them in the stack itself
 * because our stack cannot hold plain numbers.
 */
static std::vector<Ref> frames;
static size_t pos = 0;

/** Whether the stack is full. */
static inline bool full() {
    return pos == LITHP_STACK_SIZE - 1;
}
/** Start of the current frame. */
static inline Ref current_frame() {
    return frames.back();
}

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

Object **ptr() {
    return &stack[pos];
}

Ref push(Object *obj) {
    if (full()) {
        throw std::runtime_error{"stack overflow"};
    }
    stack[pos] = obj;
    // Post-increment: first call should yield 0, etc.
    return pos++;
}

Ref new_frame(Object *code) {
    stack[pos] = code;
    frames.push_back(pos);
    // Post-increment: first call should yield 0, etc.
    return pos++;
}

/** The number of function (or special form) arguments in the current frame. */
static size_t num_args_in_frame() {
    Ref fpos = current_frame();
    if (pos <= fpos) {
        throw std::logic_error{"illegal stack state"};
    }

    Ref begin_args = fpos + 1;
    size_t nargs = pos - begin_args;
    return nargs;
}

/** The  begin address of the current frame's function arguments. */
static Object **begin_args_in_frame() {
    Ref apos = current_frame() + 1;
    if (apos >= LITHP_STACK_SIZE) {
        throw std::runtime_error{"stack overflow"};
    }
    return &stack[apos];
}

void print_stack_trace() {
    std::cerr << "Stack trace:\n";
    for (auto it = frames.rbegin(); it != frames.rend(); it++) {
        std::cerr << to_string(stack[*it]) << "\n";
    }
}

void call_in_current_frame(Function *fun) {
    size_t nargs = num_args_in_frame();
    Object **args = begin_args_in_frame();
    try {
        stack[current_frame()] = fun->call(nargs, args);
    } catch (const std::exception &e) {
        std::cerr << "error calling function " + to_string(fun) << ":\n";
        std::cerr << e.what() << "\n";
        print_stack_trace();
        throw e;
    }
}

void eval_in_current_frame(SpecialForm *spec, Environment *env) {
    size_t nargs = num_args_in_frame();
    Object **args = begin_args_in_frame();
    try {
        stack[current_frame()] = spec->evaluate(nargs, args, env);
    } catch (const std::exception &e) {
        std::cerr << "error evaluating special form:\n";
        std::cerr << e.what() << "\n";
        print_stack_trace();
        throw e;
    }
}

Object *yield_frame() {
    pos = current_frame();
    Object *ret = stack[pos];
    frames.pop_back();
    return ret;
}

RefStream live_objects() {
    RefStream refs = RefStream::empty();
    for (size_t i = 0; i < pos; i++) {
        if (stack[i]) {
            RefStream orefs = stack[i]->refs();
            // Append non-empty streams
            if (orefs.has_more()) {
                refs.append(std::move(orefs));
            }
            refs.append(RefStream::of(&stack[i]));
        }
    }

    return refs;
}

void reset() {
    pos = 0;
}
} // namespace lithp::runtime::stack
