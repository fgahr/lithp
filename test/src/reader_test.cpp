#include "test_helper.hpp"

using namespace lithp;

TEST_F(RuntimeTest, read_list) {
  auto rd = getrd("(+ 1 2 3)");
  Object *obj = rd.read_expression();
  Object *result = Number::cast(eval(obj, ENV));
  Number *expected = Number::make(6);

  ASSERT_TRUE(eq(result, expected));
}

TEST_F(RuntimeTest, read_quote) {
  auto rd = getrd("'symbol");
  Object *obj = rd.read_expression();

  ASSERT_EQ(eval(obj, ENV), Symbol::intern("symbol"));
}

TEST_F(RuntimeTest, read_unbalanced_parens) {
  EXPECT_ANY_THROW(getrd("(() (+ 12 8)").read_expression());
  EXPECT_ANY_THROW(getrd(")()").read_expression());
  EXPECT_ANY_THROW(getrd("')").read_expression());
}

TEST_F(RuntimeTest, read_if_form) {
  Object *if1 = getrd("(if 1 2 3)").read_expression();
  EXPECT_TRUE(Number::eq(Number::cast(eval(if1, ENV)), Number::make(2)));
  Object *if2 = getrd("(if false 2)").read_expression();
  EXPECT_TRUE(is_null(eval(if2, ENV)));
  Object *if3 = getrd("(if true nil 1)").read_expression();
  EXPECT_TRUE(is_null(eval(if3, ENV)));
  Object *if4 = getrd("(if nil (+ 3 4) (- 3 4))").read_expression();
  EXPECT_TRUE(eq(eval(if4, ENV), Number::make(-1)));
}
