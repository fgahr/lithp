#include <object/funcall.hpp>

namespace lithp {

runtime::Allocator *Funcall::allocator = nullptr;

Type Funcall::type() { return Type::Funcall; }

Object *Funcall::eval(Environment &env) {
  if (fargs.size() < func->num_args()) {
    throw std::logic_error{
        "not enough arguments (" + std::to_string(fargs.size()) +
        ") to call function (" + std::to_string(func->num_args()) + ")"};
  } else if (fargs.size() > func->num_args() && !func->takes_rest()) {
    throw std::logic_error{
        "too many arguments (" + std::to_string(fargs.size()) +
        ") to call function (" + std::to_string(func->num_args()) + ")"};
  }

  FnArgs args = {nullptr};
  for (size_t i = 0; i < func->num_args(); i++) {
    args.at(i) = fargs.at(i)->eval(env);
  }
  return nullptr;
}

void Funcall::repr(std::ostream &out) {
  out << "(";
  func->repr(out);
  for (auto arg : fargs) {
    arg->repr(out);
  }
  out << ")";
}

RefStream Funcall::refs() {
  std::vector<Object **> arg_refs;
  for (auto &arg : fargs) {
    arg_refs.push_back(&arg);
  }

  return RefStream::concat(RefStream::of({(Object **)&func}),
                           RefStream::of(arg_refs));
}

Funcall::Funcall(Function *func, std::vector<Object *> fargs)
    : func{func}, fargs{std::move(fargs)} {}


Object *Funcall::copy_to(void *mem) { return new (mem) Funcall{func, fargs}; }
} // namespace lithp
