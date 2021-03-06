#include <functional>

#include <lithp/lithp.hpp>

namespace lithp {
class SymbolChain {
  public:
    SymbolChain(std::string name) : sym{name} {}
    Symbol *find_or_add(std::string name) {
        if (sym.name == name) {
            return &sym;
        } else if (next) {
            return next->find_or_add(name);
        } else {
            next = new SymbolChain(name);
            return &(next->sym);
        }
    }
    SymbolChain *next = nullptr;
    Symbol sym;
};

namespace {
class ChainHead {
  public:
    Symbol *find_or_add(std::string name) {
        if (chain) {
            return chain->find_or_add(name);
        }
        chain = new SymbolChain{name};
        return chain->find_or_add(name);
    }
    ~ChainHead() {
        while (chain) {
            SymbolChain *rest = chain->next;
            delete chain;
            chain = rest;
        }
    }

    SymbolChain *chain = nullptr;
};

#define LITHP_SYMBOL_TABLE_SIZE 1024

typedef std::function<void(Symbol *)> symbolAction;

class SymbolTable {
  public:
    SymbolTable() : size{LITHP_SYMBOL_TABLE_SIZE} {
        symbols.resize(size);
    }
    Symbol *create_or_get(const std::string name) {
        size_t h = std::hash<std::string_view>{}(name);
        return symbols.at(h % size).find_or_add(name);
    }
    void for_each_symbol(symbolAction action) {
        for (auto &head : symbols) {
            for (SymbolChain *c = head.chain; c != nullptr; c = c->next) {
                action(&(c->sym));
            }
        }
    }

  private:
    const size_t size;
    std::vector<ChainHead> symbols;
};
} // namespace

static SymbolTable symtab;

Symbol::Symbol(std::string name) : name{std::move(name)} {}

Type Symbol::type() {
    return Type::Symbol;
}

void Symbol::repr(std::ostream &out) {
    out << name;
}

RefStream Symbol::refs() {
    return RefStream::empty();
}

bool Symbol::self_evaluating() {
    return name.at(0) == ':';
}

std::string Symbol::get_name() {
    return name;
}

static const char *symbol_legal_chars =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:+-*/_?!";

bool Symbol::is_valid(std::string_view name) {
    return !name.empty() && name != "nil" &&
           name.find_first_not_of(symbol_legal_chars) ==
               std::string_view::npos &&
           name.find_first_of("0123456789") != 0;
}

Symbol *Symbol::intern(std::string name) {
    if (!is_valid(name)) {
        throw std::invalid_argument{"illegal symbol name '" + name + "'"};
    }
    return symtab.create_or_get(name);
}

bool Symbol::is_instance(Object *obj) {
    LITHP_CHECK_TYPE(obj, Symbol);
}

Symbol *Symbol::cast(Object *obj) {
    LITHP_CAST_TO_TYPE(obj, Symbol);
}

bool Symbol::eq(Symbol *s1, Symbol *s2) {
    return s1->name == s2->name;
}

bool Symbol::has_global_value() {
    return associated;
}
Object *Symbol::get_global_value() {
    if (associated) {
        return global_value;
    }
    return nil();
}
void Symbol::set_global_value(Object *obj) {
    associated = true;
    global_value = obj;
}

void Symbol::clear_global_associations() {
    symtab.for_each_symbol([](Symbol *sym) -> void {
        sym->associated = false;
        sym->global_value = nil();
    });
}

RefStream Symbol::global_references() {
    auto stream = RefStream::empty();
    symtab.for_each_symbol([&stream](Symbol *sym) mutable -> void {
        stream.append(RefStream::concat(sym->global_value->refs(),
                                        RefStream::of(&(sym->global_value))));
    });
    return stream;
}

} // namespace lithp
