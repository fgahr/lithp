#include <gtest/gtest.h>
#include <sstream>

#include <lithp.hpp>
#include <reader/reader.hpp>
#include <reader/tokenizer.hpp>

using namespace lithp;

class ReaderTest : public ::testing::Test {
protected:
  reader::Reader rd;
  runtime::Runtime run;
  runtime::Environment &env = run.base_env();
  virtual void SetUp() override { run.init(); }
};

std::vector<Object *> read_objects(reader::Reader &rd, const char *text) {
  std::istringstream in{text};
  return rd.read(reader::tokenize(in));
}

Object *read_single_object(reader::Reader &rd, const char *text) {
  return read_objects(rd, text)[0];
}

TEST_F(ReaderTest, list) {
  Object *obj = read_single_object(rd, "(+ 1 2 3)");
  Object *result = Number::cast(obj->evaluate(env));
  Number *expected = Number::make(6);

  ASSERT_TRUE(eq(result, expected));
}

TEST_F(ReaderTest, quote) {
  Object *obj = read_single_object(rd, "'symbol");

  ASSERT_EQ(eval(obj, env), Symbol::intern("symbol"));
}

TEST_F(ReaderTest, unbalanced_parens) {
  EXPECT_ANY_THROW(read_objects(rd, "(() (+ 12 8)"));
  EXPECT_ANY_THROW(read_objects(rd, "()))"));
  EXPECT_ANY_THROW(read_objects(rd, "')"));
}

TEST_F(ReaderTest, if_form) {
  Object *if1 = read_single_object(rd, "(if 1 2 3)");
  EXPECT_TRUE(Number::eq(Number::cast(eval(if1, env)), Number::make(2)));
  Object *if2 = read_single_object(rd, "(if false 2)");
  EXPECT_TRUE(is_null(eval(if2, env)));
  Object *if3 = read_single_object(rd, "(if true nil 1)");
  EXPECT_TRUE(is_null(eval(if3, env)));
  Object *if4 = read_single_object(rd, "(if nil (+ 3 4) (- 3 4))");
  EXPECT_TRUE(eq(eval(if4, env), Number::make(-1)));
}
