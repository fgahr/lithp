#include "test_helper.hpp"

using namespace lithp;

TEST_F(RuntimeTest, special_forms_exist) {
    EXPECT_TRUE(special::is_special(SYM("define")));
    EXPECT_TRUE(special::is_special(SYM("if")));
    EXPECT_TRUE(special::is_special(SYM("quote")));
    EXPECT_TRUE(special::is_special(SYM("set!")));
}
