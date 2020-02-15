#include <object/lambda.hpp>

namespace lithp {
namespace {
class Reference : public Object {
  LITHP_HEAP_OBJECT(Reference);

public:
  Reference(Object **obj) : ref{obj} {}
  virtual ~Reference() override = default;
  virtual size_t size() override { return sizeof(Reference); }
  virtual Type type() override { return Type::Reference; }
  virtual Object *eval(Environment &env) override {
    // We can assume that references need not be evaluated here:
    // Either they point to function slots whose contents are evaluated
    // beforehand, or they point to variables which are captured from outside.
    return *ref;
  }
  virtual void repr(std::ostream &out) override {
    throw std::logic_error{"attempting to print a Reference"};
  }
  virtual RefStream refs() override { return RefStream::of({ref}); }
  virtual Object *copy_to(void *mem) override {
    throw std::logic_error{"attempting to copy a Reference"};
  }

private:
  Object **ref;
};
} // namespace

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

Object *Lambda::call(FnArgs args, RestArgs rest) {
  // TODO
  return Object::nil();
}

bool Lambda::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Function); }

bool Lambda::eq(Lambda *l1, Lambda *l2) { return l1 == l2; }
} // namespace lithp
