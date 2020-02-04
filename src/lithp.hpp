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
  Number,
  Symbol,
  Lambda,
  BrokenHeart,
};

std::string type_name(Type t);

class Environment;

class Object;
class BrokenHeart;
class Number;
class Symbol;

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
  bool has_type(Type t);
  BrokenHeart *as_broken_heart();
  Number *as_number();
  Symbol *as_symbol();
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

class Lambda : public Object {
public:
  virtual ~Lambda() override = default;
  virtual size_t size() override { return sizeof(Lambda); }
  virtual Type type() override { return Type::Lambda; }
  virtual Object *eval(Environment &env) override;
  virtual void repr(std::ostream &out) override;
  virtual RefStream refs() override;
  virtual Object *copy_to(void *mem) override;

private:
    size_t num_args;
    std::array<Object *, 8> slots;
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
