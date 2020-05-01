#include <lithp/lithp.hpp>

namespace lithp {
Lambda *Lambda::of(size_t nargs, Object **args, Environment &env) {
    // (De-)construct argument list
    List *arglist = List::cast(args[0]);
    size_t nslots = 0;
    Symbol *rest_marker = Symbol::intern(":rest");
    Symbol *restsym = nullptr;
    for (List *list = arglist; !is_null(list); list = List::cast(cdr(list))) {
        Symbol *sym = Symbol::cast(car(list));
        if (sym == rest_marker) {
            restsym = Symbol::cast(car(List::cast(cdr(list))));
            break;
        }
        nslots++;
    }

    Symbol **slot_syms = new Symbol *[nslots];

    size_t i = 0;
    for (List *list = arglist; !is_null(list); list = List::cast(cdr(list))) {
        slot_syms[i] = Symbol::cast(car(list));
        i++;
    }

    // Extract body
    bool definitions_done = false;
    size_t progc = nargs - 1;
    Object **progv = new Object *[progc];
    for (i = 0; i < progc; i++) {
        if (!is_definition(args[i + 1])) {
            definitions_done = true;
        } else if (definitions_done && is_definition(args[i + 1])) {
            throw std::runtime_error{"definition not allowed here: " +
                                     to_string(args[i + 1])};
        }
        progv[i] = args[i + 1];
    }

    return HEAP_NEW(Lambda){
        nslots, slot_syms, restsym != nullptr, restsym, progc, progv, env};
}

Lambda::Lambda(size_t nargs, Symbol **syms, bool rest, Symbol *rest_sym,
               size_t progc, Object **progv, Environment &parent)
    : nslots{nargs}, slot_syms{syms}, has_rest{rest}, rest_sym{rest_sym},
      progc{progc}, progv{progv}, parent{parent} {}

Lambda::~Lambda() {
    delete slot_syms;
    delete progv;
}

void Lambda::repr(std::ostream &out) {
    char buffer[64] = {'\0'};
    sprintf(buffer, "<lambda#%p>", this);
    out << buffer;
}

RefStream Lambda::refs() {
    // TODO: Include program
    return parent.refs();
}

Object *Lambda::copy_to(void *) {
    throw std::logic_error{"lambda copying not yet implemented"};
}

Object *Lambda::call(size_t nargs, Object **args) {
    Environment env{&parent};
    for (size_t i = 0; i < nslots; i++) {
        env.def(slot_syms[i], args[i]);
    }
    if (has_rest) {
        env.def(rest_sym, List::of(nargs - nslots, &args[nslots]));
    }

    return eval_sequence(progc, progv, env);
}

size_t Lambda::num_slots() {
    return nslots;
}

bool Lambda::takes_rest() {
    return has_rest;
}

bool Lambda::is_instance(Object *obj) {
    LITHP_CHECK_TYPE(obj, Function);
}

bool Lambda::eq(Lambda *l1, Lambda *l2) {
    return l1 == l2;
}

} // namespace lithp
