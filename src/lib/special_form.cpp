#include <lithp.hpp>

#include "lib_util.hpp"

// TODO: Use as template, delete afterwards
#ifdef NOT_DEFINED
public:
virtual Object *eval(Environment &env) override {
  // TODO
  return nullptr;
}
virtual void repr(std::ostream &out) override {
  // TODO
}
virtual RefStream refs(void) override {
  // TODO
  return RefStream::empty();
}
virtual Object *copy_to(void *mem) override {
  // TODO
  return nullptr;
}
static SpecialForm *make(Args args) {
  // TODO
  return nullptr;
}
#endif

namespace lithp {
namespace {
using Args = std::vector<Object *>;

static void ensure_exact_args(Args &args, size_t num) {
  if (args.size() != num) {
    throw std::runtime_error{std::to_string(num) + " args required but got " +
                             std::to_string(args.size())};
  }
}

static void ensure_at_least_args(Args &args, size_t num) {
  if (args.size() < num) {
    throw std::runtime_error{std::to_string(num) +
                             " or more args required but got " +
                             std::to_string(args.size())};
  }
}

class SQuote : public SpecialForm {
  LITHP_HEAP_OBJECT(SQuote);

public:
  virtual Object *eval(Environment &) override { return quoted; }
  virtual void repr(std::ostream &out) override {
    out << '\'';
    quoted->repr(out);
  }
  virtual RefStream refs(void) override { return RefStream::of({&quoted}); }
  virtual Object *copy_to(void *mem) override {
    return new (mem) SQuote{quoted};
  }
  static SpecialForm *make(Args args) {
    ensure_exact_args(args, 1);
    return HEAP_NEW(SQuote) SQuote{args.at(0)};
  }

private:
  SQuote(Object *q) : quoted{q} {}
  Object *quoted;
};

runtime::Allocator *SQuote::allocator = nullptr;

class SDefine : public SpecialForm {
  LITHP_HEAP_OBJECT(SDefine);

public:
  virtual Object *eval(Environment &env) override {
    Object *actual = value->eval(env);
    env.set(name, actual);
    return actual;
  }
  virtual void repr(std::ostream &out) override {
    out << "(define " + name->get_name() + " ";
    value->repr(out);
    out << ")";
  }
  virtual RefStream refs(void) override {
    return RefStream::concat(RefStream::of({&value}), value->refs());
  }
  virtual Object *copy_to(void *mem) override {
    return new (mem) SDefine{name, value};
  }
  static SpecialForm *make(Args args) {
    ensure_exact_args(args, 2);
    if (!Symbol::is_instance(ARG0)) {
      throw std::runtime_error{"not a symbol: " + type_name(ARG0->type())};
    }
    return HEAP_NEW(SDefine) SDefine{Symbol::cast(ARG0), ARG1};
  }
  static RefStream all_refs(std::vector<SDefine *> defs) {
    std::vector<RefStream> streams;
    for (auto &def : defs) {
      streams.push_back(def->refs());
    }

    RefStream refs = RefStream::empty();
    for (auto &s : streams) {
      refs.append(std::move(s));
    }
    return refs;
  }

private:
  SDefine(Symbol *name, Object *value) : name{name}, value{value} {}
  Symbol *name;
  Object *value;
};

runtime::Allocator *SDefine::allocator = nullptr;

class SSet : public SpecialForm {
  LITHP_HEAP_OBJECT(SSet);

public:
  virtual Object *eval(Environment &env) override {
    if (is_null(env.lookup(name))) {
      throw std::runtime_error{"symbol not yet defined and cannot be set: " +
                               name->get_name()};
    }
    Object *actual = value->eval(env);
    env.set(name, actual);
    return actual;
  }
  virtual void repr(std::ostream &out) override {
    out << "(set! " + name->get_name() + " ";
    value->repr(out);
    out << ")";
  }
  virtual RefStream refs(void) override { return RefStream::of({&value}); }
  virtual Object *copy_to(void *mem) override {
    return new (mem) SSet{name, value};
  }
  static SpecialForm *make(Args args) {
    ensure_exact_args(args, 2);
    if (!Symbol::is_instance(ARG0)) {
      throw std::runtime_error{"not a symbol: " + type_name(ARG0->type())};
    }
    return HEAP_NEW(SSet) SSet{Symbol::cast(ARG0), ARG1};
  }

private:
  SSet(Symbol *name, Object *value) : name{name}, value{value} {}
  Symbol *name;
  Object *value;
};

runtime::Allocator *SSet::allocator = nullptr;

class SLet : public SpecialForm {
  LITHP_HEAP_OBJECT(SLet);

public:
  virtual Object *eval(Environment &env) override {
    Environment inner{&env};
    for (auto def : definitions) {
      def->eval(inner);
    }

    Object *result = nil();
    for (auto obj : body) {
      result = obj->eval(inner);
    }
    return result;
  }
  virtual void repr(std::ostream &out) override { out << "<let-form>"; }
  virtual RefStream refs(void) override {
    return RefStream::concat(SDefine::all_refs(definitions), refs_of(body));
  }
  virtual Object *copy_to(void *mem) override {
    return new (mem) SLet{definitions, body};
  }
  static SpecialForm *make(Args args) {
    ensure_at_least_args(args, 2);
    if (!List::is_instance(ARG0)) {
      throw std::runtime_error{"not a list: " + type_name(ARG0->type())};
    }
    // TODO
    return nullptr;
  }

private:
  SLet(std::vector<SDefine *> defs, std::vector<Object *> body)
      : definitions{defs}, body{body} {}
  std::vector<SDefine *> definitions;
  std::vector<Object *> body;
};

runtime::Allocator *SLet::allocator = nullptr;
} // namespace

std::unordered_map<Symbol *, builder> form_builders;

Type SpecialForm::type() { return Type::SpecialForm; }

SpecialForm *SpecialForm::dispatch(Symbol *type, Args args) {
  try {
    auto builder = form_builders.at(type);
    return builder(args);
  } catch (const std::out_of_range &) {
    throw std::logic_error{
        "dispatching special forms on non-registered symbol '" +
        type->get_name() + "'"};
  }
}

#define REGISTER_SPECIAL_FORM(class_name, symbol)                              \
  class_name::init(alloc);                                                     \
  form_builders[Symbol::intern(symbol)] = class_name::make;

void SpecialForm::init(runtime::Allocator &alloc) {
  REGISTER_SPECIAL_FORM(SQuote, "quote");
  REGISTER_SPECIAL_FORM(SDefine, "define");
  REGISTER_SPECIAL_FORM(SSet, "set!");
  REGISTER_SPECIAL_FORM(SLet, "let");
}

#undef REGISTER_SPECIAL_FORM
} // namespace lithp
