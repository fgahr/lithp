#include "test_helper.hpp"

using namespace lithp;

TEST_F(RuntimeTest, read_list) {
    Object *obj = read_from("(+ 1 2 3)");
    Object *result = Number::cast(eval(obj, ENV));
    Number *expected = Number::make(6);

    ASSERT_TRUE(eq(result, expected));
}

TEST_F(RuntimeTest, read_quote) {
    Object *obj = read_from("'symbol");

    ASSERT_EQ(eval(obj, ENV), Symbol::intern("symbol"));
}

TEST_F(RuntimeTest, read_unbalanced_parens) {
    EXPECT_ANY_THROW(read_from("(() (+ 12 8)"));
    EXPECT_ANY_THROW(read_from(")()"));
    EXPECT_ANY_THROW(read_from("')"));
}

TEST_F(RuntimeTest, read_if_form) {
    Object *if1 = read_from("(if 1 2 3)");
    EXPECT_TRUE(Number::eq(Number::cast(eval(if1, ENV)), Number::make(2)));
    Object *if2 = read_from("(if false 2)");
    EXPECT_TRUE(is_null(eval(if2, ENV)));
    Object *if3 = read_from("(if true nil 1)");
    EXPECT_TRUE(is_null(eval(if3, ENV)));
    Object *if4 = read_from("(if nil (+ 3 4) (- 3 4))");
    EXPECT_TRUE(eq(eval(if4, ENV), Number::make(-1)));
}

TEST_F(RuntimeTest, read_ignore_comment) {
    Object *sum = read_from("(+ 1 2 ; comment\n3)");
    EXPECT_EQ(eval(sum, ENV), NUM(6));
}
