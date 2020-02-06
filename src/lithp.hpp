#ifndef LITHP_LITHP_HPP
#define LITHP_LITHP_HPP

#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "stream.hpp"

namespace lithp {

enum class Type {
  Nil,
  Number,
  Symbol,
  ConsCell,
  Reference,
  Lambda,
  BrokenHeart,
};

std::string type_name(Type t);

class Environment;

class Object;
class Nil;
class Number;
class Symbol;
class ConsCell;
class Lambda;
class BrokenHeart;

using RefStream = util::Stream<Object **>;

class Object {
public:
  virtual size_t size() = 0;
  virtual Type type() = 0;
  virtual Object *eval(Environment &env) = 0;
  virtual void repr(std::ostream &out) = 0;
  virtual RefStream refs() = 0;
  virtual Object *copy_to(void *mem) = 0;
  virtual ~Object() = default;
  static bool is_nil(Object *obj);
  static Object *nil();
  bool has_type(Type t);
  BrokenHeart *as_broken_heart();
  Number *as_number();
  Symbol *as_symbol();
  Lambda *as_lambda();
};

class Nil : public Object {
  virtual size_t size() { return 0; }
  virtual Type type() { return Type::Nil; }
  virtual Object *eval(Environment &env) { return this; }
  virtual void repr(std::ostream &out) { out << "nil"; }
  virtual RefStream refs() { return RefStream::empty(); }
  virtual Object *copy_to(void *mem) {
    throw std::logic_error{"attempting to move nil"};
  }
  virtual ~Nil() = default;
};

class Number : public Object {
public:
  Number(long value);
  virtual ~Number() override = default;
  virtual size_t size() override { return sizeof(Number); }
  virtual Type type() override { return Type::Number; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;

private:
  long value;
};

class Symbol : public Object {
public:
  Symbol(std::string name);
  virtual ~Symbol() override = default;
  virtual size_t size() override { return sizeof(Symbol); }
  virtual Type type() override { return Type::Symbol; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  bool self_evaluating();
  const char *name;

private:
  Symbol(const char *name);
  static bool is_valid(std::string_view name);
  static const char *intern(std::string name);
};

class ConsCell : public Object {
public:
  ConsCell(Object *car, Object *cdr);
  virtual ~ConsCell() override = default;
  virtual size_t size() override { return sizeof(ConsCell); }
  virtual Type type() override { return Type::ConsCell; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  Object *car;
  Object *cdr;
};

class Environment {
public:
  Environment(Environment *parent = nullptr);
  void set(Symbol *sym, Object *obj);
  Object *lookup(Symbol *sym);
  Object *pull_up(Symbol *sym);
  RefStream refs();

private:
  Environment *parent;
  std::unordered_map<Symbol *, Object *> definitions;
};

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

#ifndef MAX_NUM_ARGS
#define MAX_NUM_ARGS 8
#endif

using FnSlots = std::array<Object *, MAX_NUM_ARGS>;

class Lambda : public Object {
public:
  static Lambda *of(std::vector<Symbol *> args, Symbol *rest, Object *body);
  virtual ~Lambda() override;
  virtual size_t size() override { return sizeof(Lambda); }
  virtual Type type() override { return Type::Lambda; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  Object *call(std::vector<Object *> args);

private:
  Lambda(size_t nargs, FnSlots &&slots);
  size_t num_args;
  FnSlots slots;
  Object *rest_args;
  Environment env;
  std::vector<Object *> program;
};

class BrokenHeart : public Object {
public:
  BrokenHeart(Object *redirect);
  virtual ~BrokenHeart() override = default;
  virtual size_t size() override { return redirect->size(); };
  virtual Type type() override { return Type::BrokenHeart; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;
  Object *const redirect;
};

class StackFrame {
public:
  StackFrame(StackFrame *parent, Environment env,
             std::vector<Object *> instructions);
  StackFrame() = delete;
  StackFrame(const StackFrame &frame) = delete;
  ~StackFrame() = default;
  Object *run();
  RefStream refs();
  Environment &environment();

private:
  StackFrame *parent;
  StackFrame *child = nullptr;
  Environment env;
  std::vector<Object *> program;
};

class Allocator {
public:
  Allocator(StackFrame *base_frame,
            size_t mem_size = 0x800000); // 8MB default
  Allocator() = delete;
  Allocator(const Allocator &alloc) = delete;
  ~Allocator();
  Number *allocate_number(long value);
  Symbol *allocate_symbol(std::string name);

private:
  StackFrame *base_frame;
  char *heaps[2];
  size_t heap_pos = 0;
  size_t heap_idx = 0;
  size_t mem_size;
  void *heap_ptr();
  void ensure_space(size_t amount);
  void do_gc();
  void relocate(Object **obj, char *target, size_t *pos);
  void double_heap_size();
};

} // namespace lithp

#endif /* LITHP_LITHP_HPP */
