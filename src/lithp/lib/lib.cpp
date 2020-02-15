#include <lib/lib.hpp>
#include <lithp.hpp>

#define ARG0 args[0]
#define ARG1 args[1]
#define ARG2 args[2]
#define ARG3 args[3]
#define ARG4 args[4]
#define ARG5 args[5]
#define ARG6 args[6]
#define ARG7 args[7]

using namespace lithp;

static Object *fadd(FnArgs _, RestArgs rest) {
  std::vector<Number *> nums;
  for (auto &obj : rest) {
    if (!Number::is_instance(obj)) {
      throw std::runtime_error{"not a number: " + type_name(obj->type())};
    }
    nums.push_back(static_cast<Number *>(obj));
  }

  return Number::add(nums);
}

static Object *fcons(FnArgs args, RestArgs _) {
  return ConsCell::make(ARG0, ARG1);
}

static Object *flist(FnArgs _, RestArgs rest) {
  if (rest.empty()) {
    return Object::nil();
  }

  ConsCell *head = ConsCell::make(rest.front(), Object::nil());
  ConsCell *cur = head;
  for (size_t i = 1; i < rest.size(); i++) {
    cur->car = ConsCell::make(rest.at(i), Object::nil());
  }

  return head;
}

static Object *flistp(FnArgs args, RestArgs _) {
  if (ConsCell::is_instance(ARG0)) {
    return Boolean::True();
  }
  return Boolean::False();
}

namespace lithp {
void load_stdlib(Environment &env) {
  env.set(Symbol::intern("+"), Builtin::make(0, true, fadd));
  env.set(Symbol::intern("cons"), Builtin::make(2, false, fcons));
  env.set(Symbol::intern("list"), Builtin::make(0, true, flist));
  env.set(Symbol::intern("list?"), Builtin::make(1, false, flistp));
}
} // namespace lithp
