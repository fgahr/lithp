#include <unordered_map>

#include <iostream>

#include <lithp/lithp.hpp>

#include "lib_util.hpp"

#define SYM Symbol::intern

namespace lithp {

Type SpecialForm::type() {
    return Type::SpecialForm;
}
RefStream SpecialForm::refs() {
    return RefStream::empty();
}
SpecialForm *SpecialForm::cast(Object *obj) {
    LITHP_CAST_TO_TYPE(obj, SpecialForm);
}
bool SpecialForm::is_instance(Object *obj) {
    LITHP_CHECK_TYPE(obj, SpecialForm);
}

SpecialForm::SpecialForm(size_t nargs, bool rest, snative native)
    : nslots{nargs}, has_rest{rest}, native{native} {}

namespace special {

Object *quote(size_t, Object **args, Environment *) {
    return args[0];
}

Object *if_(size_t nargs, Object **args, Environment *env) {
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

Object *define(size_t nargs, Object **args, Environment *env) {
    if (Symbol::is_instance(args[0])) {
        Symbol *sym = Symbol::cast(args[0]);
        if (env->knows(sym)) {
            throw std::runtime_error{"redefinition of variable " +
                                     to_string(sym)};
        }
        Object *value = eval(args[1], env);
        env->def(Symbol::cast(args[0]), value);
        return value;
    } else if (List::is_instance(args[0])) {
        List *decl = List::cast(args[0]);
        Symbol *sym = Symbol::cast(car(decl));
        args[0] = cdr(decl);
        Lambda *f = Lambda::of(nargs, args, env);
        env->def(sym, f);
        return f;
    } else {
        throw std::runtime_error{"malformed definition"};
    }
}

Object *set(size_t, Object **args, Environment *env) {
    Symbol *sym = Symbol::cast(args[0]);
    if (!env->knows(sym)) {
        throw std::runtime_error{"attempting to set undefined variable " +
                                 to_string(sym)};
    }
    env->set(sym, eval(args[1], env));
    return nil();
}

Object *lambda(size_t nargs, Object **args, Environment *env) {
    return Lambda::of(nargs, args, env);
}

Object *let(size_t nargs, Object **args, Environment *env) {
    List *defs = List::cast(args[0]);
    Environment *letenv = Environment::make_local(env);
    for (; !is_null(defs); defs = List::cast(cdr(defs))) {
        List *def = List::cast(car(defs));
        Symbol *sym = Symbol::cast(car(def));
        Object *value = eval(car(List::cast(cdr(def))), letenv);
        letenv->def(sym, value);
    }

    return eval_sequence(nargs - 1, &args[1], letenv);
}

Object *and_(size_t nargs, Object **args, Environment *env) {
    using namespace runtime;
    if (nargs == 0) {
        return Boolean::True();
    }

    stack::Ref evaluated = stack::push(eval(args[0], env));
    for (size_t i = 1; is_true(stack::get(evaluated)) && i < nargs; i++) {
        stack::set(evaluated, eval(args[i], env));
    }
    return stack::get(evaluated);
}

Object *or_(size_t nargs, Object **args, Environment *env) {
    using namespace runtime;
    if (nargs == 0) {
        return Boolean::False();
    }

    stack::Ref evaluated = stack::push(eval(args[0], env));
    for (size_t i = 1; is_false(stack::get(evaluated)) && i < nargs; i++) {
        stack::set(evaluated, eval(args[i], env));
    }
    return stack::get(evaluated);
}

Object *when(size_t nargs, Object **args, Environment *env) {
    if (is_false(eval(args[0], env)) || nargs == 1) {
        return nil();
    }

    return eval_sequence(nargs - 1, &args[1], env);
}

Object *unless(size_t nargs, Object **args, Environment *env) {
    if (is_true(eval(args[0], env)) || nargs == 1) {
        return nil();
    }

    return eval_sequence(nargs - 1, &args[1], env);
}
} // namespace special

static void add_builtin(Symbol *sym, SpecialForm *form) {
    runtime::global_env()->def(sym, form);
}

void SpecialForm::init(void) {
    add_builtin(SYM("define"), new SpecialForm{2, true, special::define});
    add_builtin(SYM("set!"), new SpecialForm{2, true, special::set});
    add_builtin(SYM("lambda"), new SpecialForm{2, true, special::lambda});
    add_builtin(SYM("let"), new SpecialForm{1, true, special::let});
    add_builtin(SYM("quote"), new SpecialForm{1, false, special::quote});
    add_builtin(SYM("if"), new SpecialForm{2, true, special::if_});
    add_builtin(SYM("and"), new SpecialForm{0, true, special::and_});
    add_builtin(SYM("or"), new SpecialForm{0, true, special::or_});
    add_builtin(SYM("when"), new SpecialForm{1, true, special::when});
    add_builtin(SYM("unless"), new SpecialForm{1, true, special::unless});
}

Object *SpecialForm::evaluate(size_t nargs, Object **args, Environment *env) {
    if (nargs < nslots) {
        throw std::runtime_error{
            "not enough arguments in special form: " + std::to_string(nargs) +
            " (" + std::to_string(nslots) + " required)"};
    } else if (nargs > nslots && !has_rest) {
        throw std::runtime_error{
            "too many arguments for special form: " + std::to_string(nargs) +
            " (" + std::to_string(nslots) + " required)"};
    }
    return native(nargs, args, env);
}
} // namespace lithp
