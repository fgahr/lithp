#include <gtest/gtest.h>

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
#define FUN(name) env.get_fun(Symbol::intern((name)))
#define QUOTE(form) List::of({Symbol::intern("quote"), form})

TEST_F(LibTest, functions_exist) {
  std::vector<std::string> names{"eq?", "list", "setcar!"};
  for (auto name : names) {
    Function *fun = FUN(name);

    ASSERT_TRUE(fun != nullptr);
    EXPECT_FALSE(is_null(fun));
    EXPECT_TRUE(Function::is_instance(fun));
  }
}

TEST_F(LibTest, list_functions) {
  EXPECT_TRUE(is_true(apply(FUN("null?"), List::of({nil()}), env)));
  EXPECT_TRUE(is_false(apply(FUN("null?"), List::of({SYM("cons")}), env)));

  EXPECT_TRUE(is_true(
      apply(FUN("pair?"),
            List::of({List::of({SYM("cons"), NUM(1), QUOTE(SYM("A"))})}),
            env)));
}
