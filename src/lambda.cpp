#include "lambda.hpp"

namespace lithp {
class Reference : public Object {
public:
  Reference(Object **obj);
  virtual ~Reference() override = default;
  virtual size_t size() override { return sizeof(Reference); }
  virtual Type type() override { return Type::Reference; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;

private:
  Object **ref;
};

Reference::Reference(Object **obj) : ref{obj} {}

Object *Reference::eval(Environment &env) {
  // We can assume that references need not be evaluated here:
  // Either they point to function slots whose contents are evaluated
  // beforehand, or they point to variables which are captured from outside.
  return *ref;
}

void Reference::repr(std::ostream &out) {
  throw std::logic_error{"attempting to print a Reference"};
}

RefStream Reference::refs() { return RefStream::of({ref}); }

Object *copy_to(void *mem) {
  throw std::logic_error{"attempting to copy a Reference"};
}

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

bool Lambda::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Lambda); }

} // namespace lithp
