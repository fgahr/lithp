#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#include <reader/tokens.hpp>
#include <reader/tokenizer.hpp>

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

std::vector<std::string> tokenize(std::istream &in) {
  std::vector<std::string> tokens;

  char c;
  TokenBuffer buffer;

  while (in.get(c)) {
    switch (c) {
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
    case DQUOTE:
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
