#include <object/lambda.hpp>
#include <object/nil.hpp>

namespace lithp {
// TODO: Move to separate file as publicly accessible class?
class Reference : public Object {
  LITHP_HEAP_OBJECT(Reference);

public:
  Reference(Object **obj) : ref{obj} {}
  virtual ~Reference() override = default;
  virtual Type type() override { return Type::Reference; }
  virtual Object *eval(Environment &) override {
    // We can assume that references need not be evaluated here:
    // Either they point to function slots whose contents are evaluated
    // beforehand, or they point to variables which are captured from outside.
    return *ref;
  }
  virtual void repr(std::ostream &) override {
    throw std::logic_error{"attempting to print a Reference"};
  }
  virtual RefStream refs() override { return RefStream::of({ref}); }
virtual Object *copy_to(void *) override {
    throw std::logic_error{"attempting to copy a Reference"};
  }

private:
  Object **ref;
};

runtime::Allocator *Lambda::allocator = nullptr;

Lambda *Lambda::of(std::vector<Symbol *> args, Symbol *rest, Object *body) {
  if (args.size() > MAX_NUM_ARGS) {
    throw std::logic_error{"too many function arguments (" +
                           std::to_string(args.size()) + " > " +
                           std::to_string(MAX_NUM_ARGS) + ")"};
  }
  FnSlots slots;
  // TODO
  return nullptr;
}

Lambda::~Lambda() {
  // TODO
}

void Lambda::repr(std::ostream &out) {
  char buffer[64] = {'\0'};
  sprintf(buffer, "<lambda#%p>", this);
  out << buffer;
}

RefStream Lambda::refs() {
  // TODO
  return RefStream::empty();
}

Object *Lambda::copy_to(void *mem) {
  // TODO
  throw std::logic_error{"lambda copying not yet implemented"};
}

Object *Lambda::call(SlotArgs args, RestArgs rest) {
  // TODO
  return Nil::nil();
}

size_t Lambda::num_args() { return nargs; }

bool Lambda::takes_rest() {
  // TODO
  return true;
}

bool Lambda::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Function); }

bool Lambda::eq(Lambda *l1, Lambda *l2) { return l1 == l2; }

} // namespace lithp
