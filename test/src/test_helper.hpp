#ifndef _LITHP_TEST_TEST_HELPER_H_
#define _LITHP_TEST_TEST_HELPER_H_

#include <gtest/gtest.h>

#include <sstream>

#include <lithp.hpp>
#include <reader/reader.hpp>
#include <reader/tokenizer.hpp>

class RuntimeTest : public ::testing::Test {
protected:
  virtual void SetUp() override { lithp::runtime::init(); }
  virtual void TearDown() override {
    lithp::runtime::stack::reset();
    lithp::runtime::shutdown();
  }
};

#define ENV runtime::global_env()
#define SYM Symbol::intern
#define NUM Number::make
#define FUN(name) runtime::global_env().get_fun(Symbol::intern(name))
#define QUOTE(form) List::of({Symbol::intern("quote"), form})

lithp::reader::Reader getrd(std::string text) {
  std::istringstream in{text};
  return lithp::reader::Reader{in};
}

#endif // _LITHP_TEST_TEST_HELPER_H_
