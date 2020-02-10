#include <gtest/gtest.h>

#include <object/symbol.hpp>
#include <lithp.hpp>

using namespace lithp;

Allocator allocator{nullptr, 0xf000};

TEST(symbol, create) {
    std::string name = "abcdef";
    Symbol *sym1 = allocator.allocate_symbol(name);
    Symbol *sym2 = allocator.allocate_symbol(name);

    ASSERT_TRUE(Object::eq(sym1, sym2));
}
