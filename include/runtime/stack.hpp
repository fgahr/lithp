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
 * Remove the topmost element from the stack.
 */
Object *pop();
/**
 *  Create a new stack frame for evaluating `code`.
 */
Ref new_frame(Object *code);
/**
 * Call the function `fun` in the current frame. Assumes the arguments have been
 * pushed in the current frame.
 */
void call_in_current_frame(Function *fun);
/**
 * Drop the current frame and return
 */
Object *yield_frame();

/**
 * All objects currently referenced by the stack.
 */
RefStream live_objects();
} // namespace lithp::runtime::stack

#endif // _LITHP_RUNTIME_STACK_H_
