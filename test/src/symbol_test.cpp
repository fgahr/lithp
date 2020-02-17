#include <gtest/gtest.h>

#include <object/symbol.hpp>

using namespace lithp;

TEST(symbol, eq_same) {
  std::string name = "abc-def";
  Symbol *sym1 = Symbol::intern(name);
  Symbol *sym2 = Symbol::intern(name);

  ASSERT_EQ(sym1, sym2);
}

TEST(symbol, eq_different) {
  Symbol *sym1 = Symbol::intern(":foo");
  Symbol *sym2 = Symbol::intern(":bar");

  ASSERT_NE(sym1, sym2);
}

TEST(symbol, legal) {
  EXPECT_NO_THROW(Symbol::intern("+"));
  EXPECT_NO_THROW(Symbol::intern("-"));
  EXPECT_NO_THROW(Symbol::intern("/"));
  EXPECT_NO_THROW(Symbol::intern("*global-symbol*"));
  EXPECT_NO_THROW(Symbol::intern(":1234567890_ABC"));
}

TEST(symbol, illegal) {
  EXPECT_ANY_THROW(Symbol::intern("hello\n"));
  EXPECT_ANY_THROW(Symbol::intern("world, are you there?"));
  EXPECT_ANY_THROW(Symbol::intern("9000-reasons-to-fail"));
}
