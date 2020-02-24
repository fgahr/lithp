#include <gtest/gtest.h>

#include "test_helper.hpp"

using namespace lithp;

TEST_F(RuntimeTest, lib_functions_exist) {
  std::vector<std::string> names{"eq?", "list", "setcar!"};
  for (auto name : names) {
    Function *fun = FUN(name);

    ASSERT_TRUE(fun != nullptr);
    EXPECT_FALSE(is_null(fun));
    EXPECT_TRUE(Function::is_instance(fun));
  }
}

TEST_F(RuntimeTest, lib_list_functions) {
  EXPECT_TRUE(is_true(apply(FUN("null?"), List::of({nil()}), ENV)));
  EXPECT_TRUE(is_false(apply(FUN("null?"), List::of({SYM("cons")}), ENV)));

  EXPECT_TRUE(is_true(apply(
      FUN("pair?"),
      List::of({List::of({SYM("cons"), NUM(1), QUOTE(SYM("A"))})}), ENV)));
}
