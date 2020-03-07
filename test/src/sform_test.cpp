#include "test_helper.hpp"

using namespace lithp;

TEST_F(RuntimeTest, special_forms_exist) {
  EXPECT_TRUE(SpecialForm::exists(SYM("define")));
  EXPECT_TRUE(SpecialForm::exists(SYM("if")));
  EXPECT_TRUE(SpecialForm::exists(SYM("quote")));
  EXPECT_TRUE(SpecialForm::exists(SYM("set!")));
}
