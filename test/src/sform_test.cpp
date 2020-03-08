#include "test_helper.hpp"

using namespace lithp;

TEST_F(RuntimeTest, special_forms_exist) {
  EXPECT_TRUE(SpecialForm::exists(SYM("define")));
  EXPECT_TRUE(SpecialForm::exists(SYM("if")));
  EXPECT_TRUE(SpecialForm::exists(SYM("quote")));
  EXPECT_TRUE(SpecialForm::exists(SYM("set!")));
}

TEST_F(RuntimeTest, if_works) {
  EXPECT_EQ(eval(List::of({SYM("if"), SYM("true"), NUM(0), NUM(1)}), ENV),
            NUM(0));
  EXPECT_EQ(eval(List::of({SYM("if"), nil(), NUM(0), NUM(1)}), ENV), NUM(1));
  EXPECT_EQ(eval(List::of({SYM("if"), SYM("false"), NUM(0)}), ENV), nil());
  EXPECT_EQ(eval(List::of({SYM("if"), SYM("true"), NUM(0)}), ENV), NUM(0));
}

TEST_F(RuntimeTest, quote_works) {
  EXPECT_EQ(eval(QUOTE(SYM("foo")), ENV), SYM("foo"));
  EXPECT_EQ(eval(QUOTE(NUM(20)), ENV), NUM(20));
  List *list = List::of({SYM("foo"), SYM("bar"), NUM(8)});
  EXPECT_EQ(eval(QUOTE(list), ENV), list);
}
