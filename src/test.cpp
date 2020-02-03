#include <gtest/gtest.h>

#include "lithp.hpp"

using namespace lithp;

using prog = std::vector<Object *>;

Allocator alloc{nullptr};

TEST(stack_frame, number) {
  Number *num = alloc.allocate_number(42);
  StackFrame frame{nullptr, Environment{}, prog{num}};

  ASSERT_EQ(frame.run(), num);
}

TEST(stack_frame, symbol) {
  Environment env;
  Symbol *sym = alloc.allocate_symbol("stack-frame-symbol");
  Number *num = alloc.allocate_number(13);
  env.set(sym, num);
  StackFrame frame{nullptr, env, prog{sym}};

  ASSERT_EQ(frame.run(), num);
}

TEST(object, as_number) {
  Object *num = alloc.allocate_number(13);
  EXPECT_NO_THROW(num->as_number());
  EXPECT_ANY_THROW(num->as_symbol());
  Object *sym = alloc.allocate_symbol("foo");
  EXPECT_ANY_THROW(sym->as_number());
  EXPECT_NO_THROW(sym->as_symbol());
}
