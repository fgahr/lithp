#include <gtest/gtest.h>

#include <lithp.hpp>

using namespace lithp;

TEST(env, lookup_here) {
  Environment env;
  Symbol *sym = Symbol::intern("symbol");
  Symbol *other = Symbol::intern("another");
  env.set(sym, Boolean::True());

  EXPECT_EQ(env.lookup(sym), Boolean::True());
  EXPECT_EQ(env.lookup(other), nil());
}

TEST(env, lookup_parent) {
  Environment parent;
  Environment env{&parent};
  Symbol *sym = Symbol::intern("+global-constant+");
  Symbol *other = Symbol::intern("another");

  parent.set(sym, Boolean::True());
  EXPECT_EQ(env.lookup(sym), Boolean::True());
  EXPECT_EQ(env.lookup(other), nil());
}

TEST(env, request) {
  Environment env;
  Environment other_env;
  Symbol *sym = Symbol::intern("valid/symbol");
  Object *obj = env.request(sym);

  EXPECT_EQ(env.lookup(sym), nil());
  EXPECT_EQ(obj->eval(env), nil());
  env.set(sym, Boolean::True());
  EXPECT_EQ(obj->eval(env), Boolean::True());
  // Lookup not in provided environment but where originally declared
  EXPECT_EQ(obj->eval(other_env), Boolean::True());
}
