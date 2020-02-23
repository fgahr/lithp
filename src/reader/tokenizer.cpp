#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#include <reader/tokenizer.hpp>
#include <reader/tokens.hpp>

#define BSLASH '\\'

namespace lithp::reader {
namespace {
class TokenBuffer {
public:
  void put(char c) { buffer << c; }
  void add_to(std::vector<std::string> &tokens) {
    std::string token = buffer.str();
    buffer.str("");
    if (!token.empty()) {
      tokens.push_back(token);
    }
  }

private:
  std::stringstream buffer;
};
} // namespace

std::string read_string(std::istream &in) {
  std::ostringstream out;
  char c;
  bool escaped = false;

  while (in.get(c)) {
    if (escaped) {
      escaped = false;
      out << c;
      continue;
    }
    switch (c) {
    case BSLASH:
      escaped = true;
      continue;
    case DQUOTE:
      return out.str();
    default:
      out << c;
    }
  }
  // EOF reached without closing quote
  throw std::runtime_error{"unterminated string in input sequence"};
}

std::vector<std::string> tokenize(std::istream &in) {
  std::vector<std::string> tokens;

  char c;
  TokenBuffer buffer;

  while (in.get(c)) {
    switch (c) {
    case DQUOTE:
      buffer.add_to(tokens);
      tokens.push_back(read_string(in));
      continue;
    case SPACE:
    case TAB:
    case NEWLINE:
      buffer.add_to(tokens);
      continue;

    case LPAREN:
    case RPAREN:
    case LBRACK:
    case RBRACK:
    case LBRACE:
    case RBRACE:
    case SQUOTE:
    case QQUOTE:
    case QCOMMA:
      buffer.add_to(tokens);
      tokens.push_back(std::string{c});
      continue;

    default:
      buffer.put(c);
    }
  }

  buffer.add_to(tokens);
  return tokens;
}
} // namespace lithp::reader
