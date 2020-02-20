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
    Object *fun = env.get(SYM(name));

    ASSERT_TRUE(fun != nullptr);
    EXPECT_FALSE(is_null(fun));
    EXPECT_TRUE(Function::is_instance(fun));
  }
}
