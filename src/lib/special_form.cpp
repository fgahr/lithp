#include <unordered_map>

#include <lithp.hpp>

#include "lib_util.hpp"

#define SYM Symbol::intern

namespace lithp {

static std::unordered_map<Symbol *, SpecialForm> builtins;

SpecialForm::SpecialForm(size_t nargs, bool rest, snative native)
    : nslots{nargs}, has_rest{rest}, native{native} {}

Object *squote(size_t, Object **args, Environment &) { return args[0]; }

Object *sif(size_t nargs, Object **args, Environment &env) {
  if (is_true(eval(args[0], env))) {
    return eval(args[1], env);
  } else if (nargs == 2) {
    return nil();
  } else if (nargs > 3) {
    throw std::runtime_error{"too many branches on if-expression: " +
                             std::to_string(nargs)};
  } else {
    return eval(args[2], env);
  }
}

Object *sdefine(size_t, Object **args, Environment &env) {
  if (Symbol::is_instance(args[0])) {
    Symbol *sym = Symbol::cast(args[0]);
    if (env.knows(sym)) {
      throw std::runtime_error{"redefinition of variable " + to_string(sym)};
    }
    env.def(Symbol::cast(args[0]), eval(args[1], env));
  } else if (List::is_instance(args[0])) {
    // TODO: Parse lambda
    // List *decl = List::cast(args[0]);
    // Symbol *sym = Symbol::cast(car(decl));
    // if (env.knows(sym)) {
    //   throw std::runtime_error{"redefinition of variable " + to_string(sym)};
    // }
    // List *lambda_form = cons(Symbol::intern("lambda"), cons(cdr(decl),
    // rest)); Object *lambda = eval(lambda_form, env); env.set(sym, lambda);
  } else {
    throw std::runtime_error{"malformed definition"};
  }
  return nil();
}

Object *sset(size_t, Object **args, Environment &env) {
  Symbol *sym = Symbol::cast(args[0]);
  if (!env.knows(sym)) {
    throw std::runtime_error{"attempting to set undefined variable " +
                             to_string(sym)};
  }
  env.set(sym, eval(args[1], env));
  return nil();
}

void SpecialForm::init() {
  builtins.insert_or_assign(SYM("define"), SpecialForm{2, true, sif});
  builtins.insert_or_assign(SYM("if"), SpecialForm{2, true, sif});
  builtins.insert_or_assign(SYM("quote"), SpecialForm{1, false, squote});
  builtins.insert_or_assign(SYM("set!"), SpecialForm{2, true, sset});
}

bool SpecialForm::exists(Symbol *sym) {
  if (builtins.empty()) {
    init();
  }

  return builtins.find(sym) != builtins.end();
}

SpecialForm *SpecialForm::get(Symbol *sym) {
  return &builtins.find(sym)->second;
}

Object *SpecialForm::call(size_t nargs, Object **args, Environment &env) {
  // TODO: Improve error messages
  if (nargs < nslots) {
    throw std::runtime_error{"not enough arguments in special form"};
  } else if (nargs > nslots && !has_rest) {
    throw std::runtime_error{"too many arguments for specia form"};
  }
  return native(nargs, args, env);
}
} // namespace lithp
