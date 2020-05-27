#include <iostream>
#include <sstream>

#include <lithp/lithp.hpp>

namespace lithp::runtime {

static void ensure_settable(Symbol *sym) {
    if (sym == nullptr) {
        throw std::runtime_error{"symbol is NULL"};
    } else if (sym->self_evaluating()) {
        throw std::runtime_error{"self-evaluating symbol " + to_string(sym) +
                                 " cannot be assigned to"};
    }
}

namespace {
class GlobalEnv : public Environment {
  public:
    virtual bool heap_allocated() override {
        return false;
    }
    virtual size_t size(void) override {
        return 0;
    }
    virtual Type type(void) override {
        return Type::Environment;
    }
    virtual void repr(std::ostream &out) override {
        char buffer[64] = {'\0'};
        sprintf(buffer, "<global-env#%p>", this);
        out << buffer;
    }
    virtual RefStream refs(void) override {
        return Symbol::global_references();
    }
    virtual Object *copy_to(void *) override {
        throw std::logic_error{"attempting to copy global environment"};
    }
    virtual void set(Symbol *sym, Object *obj) override {
        if (sym->has_global_value()) {
            sym->set_global_value(obj);
        } else {
            throw std::runtime_error{
                "attempting to change undefined variable: " + sym->get_name()};
        }
    }
    virtual void def(Symbol *sym, Object *obj) override {
        if (sym->has_global_value()) {
            throw std::runtime_error{"redefining global variable: " +
                                     sym->get_name()};
        } else {
            sym->set_global_value(obj);
        }
    }
    virtual Object *get(Symbol *sym) override {
        if (sym->has_global_value()) {
            return sym->get_global_value();
        } else {
            throw std::runtime_error{"unknown variable: " + sym->get_name()};
        }
    }
    virtual bool knows(Symbol *sym) override {
        return sym->has_global_value();
    }
    virtual ~GlobalEnv() = default;

    static GlobalEnv global;
};

GlobalEnv GlobalEnv::global = GlobalEnv{};

typedef struct {
    Symbol *sym;
    Object *val;
} Def;

class LocalEnv : public Environment {
  public:
    LocalEnv(Environment *parent) : parent{parent} {}
    virtual bool heap_allocated() override {
        return true;
    }
    virtual size_t size(void) override {
        return sizeof(LocalEnv);
    }
    virtual Type type(void) override {
        return Type::Environment;
    }
    virtual void repr(std::ostream &out) override {
        char buffer[64] = {'\0'};
        sprintf(buffer, "<env#%p>", this);
        out << buffer;
    }
    virtual RefStream refs(void) override {
        std::vector<Object **> ptrs;
        for (size_t i = 0; i < ndefs; i++) {
            ptrs.push_back(&(definitions[i].val));
        }
        return RefStream::concat(parent->refs(),
                                 RefStream::of((Object **)(&parent)),
                                 RefStream::of(ptrs));
    }
    virtual Object *copy_to(void *mem) override {
        LocalEnv *copy = new (mem) LocalEnv{this->parent};
        for (size_t i = 0; i < ndefs; i++) {
            copy->definitions[i] = this->definitions[i];
        }
        return copy;
    }
    virtual void set(Symbol *sym, Object *obj) override {
        ensure_settable(sym);

        for (size_t i = 0; i < ndefs; i++) {
            if (definitions[i].sym == sym) {
                definitions[i].val = obj;
                return;
            }
        }
        parent->set(sym, obj);
    }
    virtual void def(Symbol *sym, Object *obj) override {
        ensure_settable(sym);

        if (knows(sym)) {
            throw std::runtime_error{"double definition of variable: " +
                                     sym->get_name()};
        } else if (ndefs == max_ndefs) {
            throw std::runtime_error{"environment exceeded max size: " +
                                     std::to_string(max_ndefs)};
        } else {
            definitions[ndefs++] = Def{.sym = sym, .val = obj};
        }
    }
    virtual Object *get(Symbol *sym) override {
        for (size_t i = 0; i < ndefs; i++) {
            if (definitions[i].sym == sym) {
                return definitions[i].val;
            }
        }
        return parent->get(sym);
    }
    virtual bool knows(Symbol *sym) override {
        for (size_t i = 0; i < ndefs; i++) {
            if (definitions[i].sym == sym) {
                return true;
            }
        }
        return false;
    }
    virtual ~LocalEnv() = default;

  private:
    static const size_t max_ndefs = 32;
    Environment *parent;
    size_t ndefs = 0;
    Def definitions[max_ndefs] = {Def{nullptr, nullptr}};
};
} // namespace

Environment *Environment::get_global() {
    return &GlobalEnv::global;
}
Environment *Environment::make_local(Environment *parent) {
    return HEAP_NEW(LocalEnv){parent};
}
} // namespace lithp::runtime
