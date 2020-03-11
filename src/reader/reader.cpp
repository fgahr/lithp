#include <optional>
#include <regex>
#include <string>

#include <lithp.hpp>
#include <reader/reader.hpp>
#include <reader/tokens.hpp>

namespace lithp::reader {
using Token = std::string;

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

static Token read_string(std::istream &in) {
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
    case BSL:
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

// TODO: Use in reader
std::optional<Token> next_token(std::istream &in) {
  if (in.eof()) {
    return std::nullopt;
  }
  char c;
  std::ostringstream buffer;

  while (in.get(c)) {
    switch (c) {
    case DQUOTE:
      return read_string(in);
    case SPC:
    case TAB:
    case NWL:
      if (auto token = buffer.str(); !token.empty()) {
        return std::make_optional(token);
      } else {
        continue;
      }

    case LPAREN:
    case RPAREN:
    case LBRACK:
    case RBRACK:
    case LBRACE:
    case RBRACE:
    case SQUOTE:
    case QQUOTE:
    case QCOMMA:
      if (auto token = buffer.str(); !token.empty()) {
        in.unget();
        return std::make_optional(token);
      } else {
        return std::make_optional(std::string{c});
      }

    default:
      buffer << c;
    }
  }

  if (auto token = buffer.str(); !token.empty()) {
    return std::make_optional(token);
  } else {
    return std::nullopt;
  }
}

std::vector<Token> tokenize(std::istream &in) {
  std::vector<Token> tokens;

  char c;
  TokenBuffer buffer;

  while (in.get(c)) {
    switch (c) {
    case DQUOTE:
      buffer.add_to(tokens);
      tokens.push_back(read_string(in));
      continue;
    case SPC:
    case TAB:
    case NWL:
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
namespace lithp::reader {

using TokenStream = util::Stream<Token>;
using ObjectStream = util::Stream<Object *>;

class Parser;

class Reader {
public:
  Reader(std::istream &in);
  ~Reader();
  Reader() = delete;
  Reader(const Reader &other) = delete;
  Object *parse_next(Token token, TokenStream &tokens);
  Object *parse_next(TokenStream &tokens);
  Object *read_expression();
  bool at_eof();

private:
  Parser &parser_for_token(const Token &token);
  std::vector<Parser *> parsers;
  std::istream &in;
};

class Parser {
public:
  virtual ~Parser() = default;
  virtual bool relevant(const Token &token) = 0;
  virtual Object *parse(Token first, TokenStream &rest) = 0;
};

namespace {
class NilParser : public Parser {
public:
  virtual bool relevant(const Token &token) override { return token == "nil"; }
  virtual Object *parse(Token, TokenStream &) override { return nil(); }
};

class NumberParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    std::regex number{"-?[0-9]+"};
    return std::regex_match(token, number);
  }
  virtual Number *parse(Token first, TokenStream &) override {
    return Number::make(std::stol(first));
  }
};

class SymbolParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    return Symbol::is_valid(token);
  }
  virtual Symbol *parse(Token first, TokenStream &) override {
    return Symbol::intern(first);
  }
};

class ListParser : public Parser {
public:
  ListParser(Reader *reader) : reader{reader} {}
  virtual bool relevant(const Token &token) override {
    return token == S_LPAREN;
  }
  virtual List *parse(Token, TokenStream &tokens) override {
    Token token;
    List *head = nullptr;
    List *current = nullptr;
    // FIXME: Will crash with not-helpful error message if parens are unbalanced
    while ((token = tokens.get()) != S_RPAREN) {
      if (head == nullptr) {
        head = List::make(reader->parse_next(token, tokens), nil());
        current = head;
      } else {
        List *next = cons(reader->parse_next(token, tokens), nil());
        current->set_cdr(next);
        current = next;
      }
    }

    return head;
  }

private:
  Reader *reader;
};

class QuoteParser : public Parser {
public:
  QuoteParser(Reader *reader) : reader{reader} {
    quote = Symbol::intern("quote");
  }
  virtual bool relevant(const std::string &token) override {
    return token == S_SQUOTE;
  }

  virtual Object *parse(Token, TokenStream &tokens) override {
    return List::of({quote, reader->parse_next(tokens)});
  }

private:
  Symbol *quote;
  Reader *reader;
};
} // namespace

Reader::Reader(std::istream &in) : in{in} {
  parsers = {new NilParser(), new NumberParser(), new SymbolParser(),
             new ListParser{this}, new QuoteParser{this}};
}

Reader::~Reader() {
  for (auto ptr : parsers) {
    delete ptr;
  }
}

Object *Reader::parse_next(Token token, TokenStream &tokens) {
  Parser &p = parser_for_token(token);
  return p.parse(token, tokens);
}

Object *Reader::parse_next(TokenStream &tokens) {
  Token token = tokens.get();
  return parse_next(token, tokens);
}

Object *Reader::read_expression() { return parse_next(tokens); }

bool Reader::at_eof() { return in.eof(); }

Parser &Reader::parser_for_token(const std::string &token) {
  for (auto parser : parsers) {
    if (parser->relevant(token)) {
      return *parser;
    }
  }
  throw std::runtime_error{"token not recognized: " + token};
}

static Reader *rd = nullptr;

void init(std::istream &in) { rd = new Reader{in}; }
bool done() { return rd->at_eof(); }
void exit() { delete rd; }
Object *next_expr() { return rd->read_expression(); }

} // namespace lithp::reader
