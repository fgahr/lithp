#ifndef _LITHP_RUNTIME_STACK_H_
#define _LITHP_RUNTIME_STACK_H_

#include <object.hpp>
#include <object/function.hpp>
#include <object/special_form.hpp>

namespace lithp::runtime::stack {
using Ref = size_t;
/**
 * Get the referenced stack object.
 */
Object *get(Ref ref);
/**
 * Place `obj` at the given stack position.
 */
void set(Ref ref, Object *obj);
/**
 * The current stack pointer.
 */
Object **ptr();
/**
 * Push `obj` to the stack.
 */
Ref push(Object *obj);
/**
 *  Create a new stack frame for evaluating `code`.
 */
Ref new_frame(Object *code);
/**
 * Print the current stack trace.
 */
void print_stack_trace();
/**
 * Call the function `fun` in the current frame with the previously emplaced
 * arguments.
 */
void call_in_current_frame(Function *fun);
/**
 * Evaluate the special form `spec` in the current frame with the previously
 * emplaced arguments. Can make use of and manipulate `env`.
 */
void eval_in_current_frame(SpecialForm *spec, Environment &env);
/**
 * Drop the current frame and return
 */
Object *yield_frame();
/**
 * A stream of objects currently referenced by the stack.
 */
// FIXME: Quite inefficient in terms of memory (linked list but worse).
// Fixing might require breaking abstraction or a more specialized stream.
RefStream live_objects();
/**
 * Reset the stack to initial conditions.
 */
void reset();
} // namespace lithp::runtime::stack

#endif // _LITHP_RUNTIME_STACK_H_
