#include <gtest/gtest.h>

#include "test_helper.hpp"

using namespace lithp;

TEST_F(RuntimeTest, lib_functions_exist) {
    std::vector<std::string> names{"eq?", "list", "set-car!"};
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

TEST_F(RuntimeTest, lib_nested_functions) {
    EXPECT_EQ(
        eval(List::of({SYM("*"), List::of({SYM("+"), NUM(1), NUM(2)}), NUM(4)}),
             ENV),
        NUM(12));
}

TEST_F(RuntimeTest, lib_car_cdr) {
    List *list = QUOTE(List::make(NUM(1), NUM(2)));
    ASSERT_EQ(eval(List::of({SYM("car"), list}), ENV), (Object *)NUM(1));
    ASSERT_EQ(eval(List::of({SYM("cdr"), list}), ENV), (Object *)NUM(2));
}

TEST_F(RuntimeTest, lib_set_car_cdr) {
    Environment *env = Environment::make_local(ENV);
    List *list = List::make(SYM("A"), SYM("B"));
    Symbol *listsym = SYM("my-list");
    env->def(listsym, list);
    ASSERT_NE(eval(listsym, env), nil());
    ASSERT_EQ(eval(List::of({SYM("car"), listsym}), env), SYM("A"));
    eval(List::of({SYM("set-car!"), listsym, QUOTE(SYM("C"))}), env);
    EXPECT_EQ(eval(List::of({SYM("car"), listsym}), env), SYM("C"));
    eval(List::of({SYM("set-cdr!"), listsym, QUOTE(SYM("D"))}), env);
    EXPECT_EQ(eval(List::of({SYM("cdr"), listsym}), env), SYM("D"));
}
