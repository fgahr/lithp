#include <gtest/gtest.h>

#include <lithp.hpp>

using namespace lithp;

TEST(env, lookup_here) {
  Environment env;
  Symbol *sym = Symbol::intern("symbol");
  Symbol *other = Symbol::intern("another");
  env.set(sym, Boolean::True());

  EXPECT_EQ(env.get(sym), Boolean::True());
  EXPECT_EQ(env.get(other), nil());
}

TEST(env, lookup_parent) {
  Environment parent;
  Environment env{&parent};
  Symbol *sym = Symbol::intern("+global-constant+");
  Symbol *other = Symbol::intern("another");

  parent.set(sym, Boolean::True());
  EXPECT_EQ(env.get(sym), Boolean::True());
  EXPECT_EQ(env.get(other), nil());
}
