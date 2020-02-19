#include <sstream>

#include <lithp.hpp>

namespace lithp::runtime {

namespace {
class Placeholder : public Object {
public:
  Placeholder(Symbol *sym, Environment *env) : sym{sym}, orig_env{env} {}
  virtual Type type(void) override { return Type::Placeholder; }
  virtual Object *eval(Environment &) override { return orig_env->lookup(sym); }
  virtual void repr(std::ostream &) override {
    throw std::runtime_error{"attempting to print unevaluated placeholder"};
  }
  virtual RefStream refs(void) override { return RefStream::empty(); }
  virtual Object *copy_to(void *) override {
    throw std::logic_error{"attempting to copy non-heap object PlaceHolder"};
  }
  static bool is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Placeholder); }

private:
  Symbol *sym;
  Environment *orig_env;
};
} // namespace

Environment::Environment(Environment *parent) : parent{parent} {}

void Environment::set(Symbol *sym, Object *obj) {
  // NOTE: Check whether symbol is self-evaluating cannot be done here.
  // Hence it needs to be done beforehand.
  definitions.insert_or_assign(sym, obj);
}

Object *Environment::lookup(Symbol *sym) {
  auto found = definitions.find(sym);
  if (found != definitions.end()) {
    Object *val = found->second;
    if (Placeholder::is_instance(val)) {
      return nil();
    }
    return val;
  }

  if (parent) {
    return parent->lookup(sym);
  } else {
    return nil();
  }
}

Object *Environment::request(Symbol *sym) {
  auto found = lookup(sym);
  if (is_null(found)) {
    Placeholder *p = new Placeholder(sym, this);
    set(sym, p);
    return p;
  } else {
    return found;
  }
}

// TODO: Might make no sense to have this, delete?
Object *Environment::pull_up(Symbol *sym) {
  Object *obj = lookup(sym);
  definitions.insert_or_assign(sym, obj);
  return obj;
}

RefStream Environment::refs() { return refs_of(definitions); }
} // namespace lithp::runtime
