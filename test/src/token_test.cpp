#include <gtest/gtest.h>
#include <sstream>

#include <reader/tokenizer.hpp>

using namespace lithp::reader;

std::vector<std::string> get_tokens(std::string input) {
  auto in = std::istringstream{input};
  return tokenize(in);
}

TEST(token, none) {
  auto no_tokens = get_tokens(" \t\t\n   ");

  EXPECT_TRUE(no_tokens.empty());
}

TEST(token, words) {
  auto words = get_tokens("abc def+123\tghi-jkl/mno\np ");

  ASSERT_EQ(words.size(), 4);
  EXPECT_EQ(words.at(0), "abc");
  EXPECT_EQ(words.at(1), "def+123");
  EXPECT_EQ(words.at(2), "ghi-jkl/mno");
  EXPECT_EQ(words.at(3), "p");
}

TEST(token, quotes) {
  auto tokens = get_tokens("'A ''' '-");

  EXPECT_EQ(tokens.size(), 7);
  EXPECT_EQ(tokens.at(0), "'");
  EXPECT_EQ(tokens.at(1), "A");
  EXPECT_EQ(tokens.at(2), "'");
  EXPECT_EQ(tokens.at(3), "'");
  EXPECT_EQ(tokens.at(4), "'");
  EXPECT_EQ(tokens.at(5), "'");
  EXPECT_EQ(tokens.at(6), "-");
}

TEST(token, various_delimiters) {
  auto tokens = get_tokens("(` ')\t}{[ ,)`'");

  ASSERT_EQ(tokens.size(), 11);
}

TEST(token, small_list) {
  auto tokens = get_tokens("'(a b c)");

  ASSERT_EQ(tokens.size(), 6);
  EXPECT_EQ(tokens.at(0), "'");
  EXPECT_EQ(tokens.at(1), "(");
  EXPECT_EQ(tokens.at(2), "a");
  EXPECT_EQ(tokens.at(3), "b");
  EXPECT_EQ(tokens.at(4), "c");
  EXPECT_EQ(tokens.at(5), ")");
}
