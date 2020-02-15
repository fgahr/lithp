#ifndef LITHP_LITHP_HPP
#define LITHP_LITHP_HPP

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <object.hpp>
#include <object/boolean.hpp>
#include <object/broken_heart.hpp>
#include <object/builtin.hpp>
#include <object/cons_cell.hpp>
#include <object/funcall.hpp>
#include <object/function.hpp>
#include <object/lambda.hpp>
#include <object/number.hpp>
#include <object/symbol.hpp>
#include <runtime/allocator.hpp>
#include <runtime/environment.hpp>
#include <util/refstream.hpp>

namespace lithp {

void print(Object *obj, std::ostream &out);

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

class Interpreter {
public:
  void init();

private:
  runtime::Allocator allocator;
};

} // namespace lithp

#endif /* LITHP_LITHP_HPP */
