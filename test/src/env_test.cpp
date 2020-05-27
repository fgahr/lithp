#include <gtest/gtest.h>

#include <lithp/lithp.hpp>

#include "test_helper.hpp"

using namespace lithp;

TEST_F(RuntimeTest, env_lookup_here) {
    Environment *env = Environment::make_local(ENV);
    Symbol *sym = Symbol::intern("symbol");
    Symbol *other = Symbol::intern("another");
    env->def(sym, Boolean::True());

    EXPECT_EQ(env->get(sym), Boolean::True());
    EXPECT_ANY_THROW(env->get(other));
}

TEST_F(RuntimeTest, env_lookup_parent) {
    Environment *parent = Environment::make_local(ENV);
    Environment *env = Environment::make_local(parent);
    Symbol *sym = Symbol::intern("+global-constant+");
    Symbol *other = Symbol::intern("another");

    parent->def(sym, Boolean::True());
    EXPECT_EQ(env->get(sym), Boolean::True());
    EXPECT_ANY_THROW(env->get(other));
}
