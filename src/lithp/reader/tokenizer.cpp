#include <sstream>
#include <stdexcept>
#include <string>

#include <reader/tokenizer.hpp>

#define SPACE ' '
#define TAB '\t'
#define NEWLINE '\n'

#define LPAREN '('
#define RPAREN ')'
#define LBRACK '['
#define RBRACK ']'
#define LBRACE '{'
#define RBRACE '}'
#define SQUOTE '\''
#define DQUOTE '"'

namespace lithp::reader {

namespace {
class TokenBuffer {
public:
  bool empty() { return !buffer.rdbuf()->in_avail(); }
  void put(char c) { buffer << c; }
  std::string emit() {
    if (empty()) {
      throw std::logic_error{"call to emit() with empty buffer"};
    }
    std::string token;
    buffer >> token;
    return token;
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
      if (!buffer.empty()) {
        tokens.push_back(buffer.emit());
      }
      continue;

    case LPAREN:
    case RPAREN:
    case LBRACK:
    case RBRACK:
    case LBRACE:
    case RBRACE:
    case SQUOTE:
    case DQUOTE:
      if (!buffer.empty()) {
        tokens.push_back(buffer.emit());
      }
      tokens.push_back(std::to_string(c));
      continue;
    default:
      buffer.put(c);
    }
  }
  return tokens;
}
} // namespace lithp::reader
