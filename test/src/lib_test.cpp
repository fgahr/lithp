#include <gtest/gtest.h>

#include <lib/stdlib.hpp>
#include <lithp.hpp>

using namespace lithp;

class LibTest : public ::testing::Test {
protected:
  runtime::Runtime run;
  runtime::Environment &env = run.base_env();
  virtual void SetUp() override { run.init(); }
};

#define SYM Symbol::intern
#define NUM Number::make

TEST_F(LibTest, functions_exist) {
  std::vector<std::string> names{"eq?", "list", "setcar!"};
  for (auto name : names) {
    Object *fun = env.lookup(SYM(name));

    ASSERT_TRUE(fun != nullptr);
    EXPECT_FALSE(is_null(fun));
    EXPECT_TRUE(Function::is_instance(fun));
  }
}

#define GET_FUN(name) Function::cast(env.lookup(SYM(name)))
#define FN_CALL Funcall::make

TEST_F(LibTest, symbol_resolve) {
  Function *add = GET_FUN("+");
  env.set(SYM("A"), NUM(2));
  Funcall *call_add = FN_CALL(add, List::of({NUM(1), SYM("A")}));
  env.set(SYM("result"), call_add->eval(env));

  Function *eq = GET_FUN("eq?");
  Funcall *call_eq = FN_CALL(eq, List::of({NUM(3), SYM("result")}));

  EXPECT_EQ(Boolean::True(), call_eq->eval(env));
}

TEST_F(LibTest, nested_call) {
  Function *list = GET_FUN("list");
  Function *pair = GET_FUN("pair?");
  Funcall *list_call = FN_CALL(list, List::of({NUM(1), NUM(2), NUM(3)}));
  Funcall *pair_call = FN_CALL(pair, List::of({list_call}));

  EXPECT_EQ(Boolean::True(), pair_call->eval(env));
}
