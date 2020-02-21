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
    Number *result = Number::cast(obj->evaluate(env));
    Number *expected = Number::make(6);

    ASSERT_TRUE(Number::eq(result, expected));
}
