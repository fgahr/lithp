#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#include "lithp.hpp"

namespace lithp {

static RefStream refs_of(std::vector<Object *> &objs) {
  std::vector<Object **> refs{objs.size()};
  size_t pos = 0;

  for (auto &o : objs) {
    refs[pos++] = &o;
  }

  return RefStream::of(refs);
}

static RefStream refs_of(std::unordered_map<Symbol *, Object *> &pairs) {
  std::vector<Object **> refs{2 * pairs.size()};
  size_t pos = 0;

  for (auto &[key, value] : pairs) {
    refs[pos++] = (Object **)&key;
    refs[pos++] = &value;
  }

  return RefStream::of(refs);
}

// Object //////////////////////////////////////////////////////////////////////

#define TYPE_NAME_CASE(t)                                                      \
  case Type::t:                                                                \
    return #t

std::string type_name(Type t) {
  switch (t) {
    TYPE_NAME_CASE(Number);
    TYPE_NAME_CASE(Symbol);
    TYPE_NAME_CASE(Lambda);
    TYPE_NAME_CASE(BrokenHeart);
  default:
    throw std::logic_error{"unknown type " +
                           std::to_string(static_cast<int>(t))};
  }
}

#undef TYPE_NAME_CASE

bool Object::has_type(Type t) { return type() == t; }

#define CONVERT_TYPE(t)                                                        \
  if (!has_type(Type::t)) {                                                    \
    throw std::logic_error{"illegal type conversion from " +                   \
                           type_name(type()) + " to " + type_name(Type::t)};   \
  }                                                                            \
  return static_cast<t *>(this)

BrokenHeart *Object::as_broken_heart() { CONVERT_TYPE(BrokenHeart); }

Number *Object::as_number() { CONVERT_TYPE(Number); }

Symbol *Object::as_symbol() { CONVERT_TYPE(Symbol); }

#undef CONVERT_TYPE

// Number //////////////////////////////////////////////////////////////////////

Number::Number(long value) : value{value} {}

Object *Number::eval(Environment &env) { return this; }

void Number::repr(std::ostream &out) { out << value; }

RefStream Number::refs() { return RefStream::empty(); }

Object *Number::copy_to(void *mem) { return new (mem) Number{this->value}; }

// Symbol //////////////////////////////////////////////////////////////////////

namespace {
class SymbolTable {
public:
  const char *create_or_get(const std::string name) {
    auto it = symbols.insert(name);
    return it.first->c_str();
  }

private:
  std::unordered_set<std::string> symbols;
};
} // namespace

static SymbolTable symtab;

Symbol::Symbol(const char *name) : name{name} {}

Symbol::Symbol(std::string name) { this->name = intern(name); }

Object *Symbol::eval(Environment &env) {
  if (self_evaluating()) {
    return this;
  }
  return env.lookup(this);
}

void Symbol::repr(std::ostream &out) { out << name; }

RefStream Symbol::refs() { return RefStream::empty(); }

Object *Symbol::copy_to(void *mem) { return new (mem) Symbol{name}; }

bool Symbol::self_evaluating() { return name[0] == ':'; }

static const char *symbol_legal_chars =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-/_?!";

bool Symbol::is_valid(std::string_view name) {
  return !name.empty() &&
         name.find_first_not_of(symbol_legal_chars) == std::string_view::npos &&
         name.find_first_of("0123456789") != 0;
}

const char *Symbol::intern(std::string name) {
  if (!is_valid(name)) {
    throw std::invalid_argument{"illegal symbol name '" + name + "'"};
  }
  return symtab.create_or_get(name);
}

// Lambda //////////////////////////////////////////////////////////////////////

// TODO

// BrokenHeart /////////////////////////////////////////////////////////////////

BrokenHeart::BrokenHeart(Object *redirect) : redirect{redirect} {}

Object *BrokenHeart::eval(Environment &env) {
  throw std::logic_error{"attempting to evaluate a BrokenHeart"};
}

void BrokenHeart::repr(std::ostream &out) {
  throw std::logic_error{"attempting to print a BrokenHeart"};
}

RefStream BrokenHeart::refs() {
  throw std::logic_error{"attempting to get refs of a BrokenHeart"};
}

Object *BrokenHeart::copy_to(void *mem) {
  throw std::logic_error{"attempting to copy a BrokenHeart"};
}

Environment::Environment(Environment *parent) : parent{parent} {}

void Environment::set(Symbol *sym, Object *obj) {
  if (sym->self_evaluating()) {
    throw std::invalid_argument{"self-evaluating symbol " +
                                std::string{sym->name} +
                                " cannot be assigned to"};
  }
  definitions.insert_or_assign(sym, obj);
}

Object *Environment::lookup(Symbol *sym) {
  auto found = definitions.find(sym);
  if (found != definitions.end()) {
    return found->second;
  }

  if (parent) {
    return parent->lookup(sym);
  } else {
    std::ostringstream error{"unknown symbol "};
    error << sym->name;
    throw std::invalid_argument{error.str()};
  }
}

Object *Environment::pull_up(Symbol *sym) {
  Object *obj = lookup(sym);
  definitions.insert_or_assign(sym, obj);
  return obj;
}

RefStream Environment::refs() { return refs_of(definitions); }

StackFrame::StackFrame(StackFrame *parent, Environment env,
                       std::vector<Object *> instructions)
    : parent{parent}, env{std::move(env)}, program{instructions} {
  if (parent) {
    env = Environment{parent->env};
  } else {
    env = Environment{nullptr};
  }
}

Object *StackFrame::run() {
  Object *ret;
  for (Object *obj : program) {
    ret = obj->eval(env);
  }
  return ret;
}

RefStream StackFrame::refs() {
  RefStream child_refs = child ? child->refs() : RefStream::empty();
  return RefStream::concat(RefStream::concat(env.refs(), refs_of(program)),
                           std::move(child_refs));
}

Environment &StackFrame::environment() { return env; }

Allocator::Allocator(StackFrame *base_frame, size_t mem_size)
    : base_frame{base_frame}, mem_size{mem_size} {
  heaps[0] = (char *)std::calloc(mem_size, sizeof(char));
  heaps[1] = (char *)std::calloc(mem_size, sizeof(char));

  if (!heaps[0] || !heaps[1]) {
    throw std::runtime_error{"memory allocation failed"};
  }
}

Allocator::~Allocator() {
  if (heaps[0]) {
    free(heaps[0]);
  }
  if (heaps[1]) {
    free(heaps[1]);
  }
}

Number *Allocator::allocate_number(long value) {
  const size_t size = sizeof(Number);
  ensure_space(size);
  auto num = new (heap_ptr()) Number{value};
  heap_pos += size;
  return num;
}

Symbol *Allocator::allocate_symbol(std::string name) {
  const size_t size = sizeof(Symbol);
  ensure_space(size);
  auto sym = new (heap_ptr()) Symbol(name);
  heap_pos += size;
  return sym;
}

void *Allocator::heap_ptr() { return &heaps[heap_idx][heap_pos]; }

void Allocator::ensure_space(size_t amount) {
  if (heap_pos + amount >= mem_size) {
    do_gc();
    if (heap_pos > 0.8 * mem_size) { // heap more than 80% full
      double_heap_size();
    }
  }
}

void Allocator::do_gc() {
  RefStream refs = base_frame->refs();
  char *new_heap = heaps[!heap_idx];
  size_t pos = 0;

  while (refs.has_more()) {
    relocate(refs.get(), new_heap, &pos);
  }

  // Call destructors for obsolete objects
  size_t size;
  while (pos < mem_size) {
    Object *obj = (Object *)&heaps[heap_idx][pos];
    size = obj->size();
    delete obj;
    pos += size;
  }

  // Use new heap
  heap_idx = !heap_idx;
}

void Allocator::relocate(Object **obj, char *target, size_t *pos) {
  if ((*obj)->has_type(Type::BrokenHeart)) {
    *obj = (*obj)->as_broken_heart()->redirect;
    return;
  }

  Object *moved = (*obj)->copy_to(&target[*pos]);
  void *old_loc = *obj;
  delete *obj;
  new (old_loc) BrokenHeart{moved};
  *obj = moved;
  *pos += moved->size();
}

void Allocator::double_heap_size() {
  mem_size *= 2;
  heaps[0] = (char *)realloc(heaps[0], mem_size);

  if (!heaps[0] || !heaps[1]) {
    throw std::runtime_error{"memory allocation failed"};
  }
}

} // namespace lithp
