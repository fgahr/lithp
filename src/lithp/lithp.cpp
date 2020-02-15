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


void Interpreter::init() {
  ConsCell::init(allocator);
  Funcall::init(allocator);
  Lambda::init(allocator);
  Number::init(allocator);
}

} // namespace lithp
