#include <gtest/gtest.h>

#include <lithp/lithp.hpp>

using namespace lithp;

TEST(env, lookup_here) {
    Environment env;
    Symbol *sym = Symbol::intern("symbol");
    Symbol *other = Symbol::intern("another");
    env.def(sym, Boolean::True());

    EXPECT_EQ(env.get(sym), Boolean::True());
    EXPECT_ANY_THROW(env.get(other));
}

TEST(env, lookup_parent) {
    Environment parent;
    Environment env{&parent};
    Symbol *sym = Symbol::intern("+global-constant+");
    Symbol *other = Symbol::intern("another");

    parent.def(sym, Boolean::True());
    EXPECT_EQ(env.get(sym), Boolean::True());
    EXPECT_ANY_THROW(env.get(other));
}
