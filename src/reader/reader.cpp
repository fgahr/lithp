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

using ObjectStream = util::Stream<Object *>;

class Parser;

class Reader {
public:
  Reader(std::istream &in);
  ~Reader();
  Reader() = delete;
  Reader(const Reader &other) = delete;
  Token get_token();
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
  virtual Object *parse(Token first, Reader *r) = 0;
};

namespace {
class NilParser : public Parser {
public:
  virtual bool relevant(const Token &token) override { return token == "nil"; }
  virtual Object *parse(Token, Reader *r) override { return nil(); }
};

class NumberParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    std::regex number{"-?[0-9]+"};
    return std::regex_match(token, number);
  }
  virtual Number *parse(Token first, Reader *) override {
    return Number::make(std::stol(first));
  }
};

class SymbolParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    return Symbol::is_valid(token);
  }
  virtual Symbol *parse(Token first, Reader *) override {
    return Symbol::intern(first);
  }
};

class ListParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    return token == S_LPAREN;
  }
  virtual List *parse(Token, Reader *r) override {
    Token token;
    List *head = nullptr;
    List *current = nullptr;
    while ((token = r->get_token()) != S_RPAREN) {
      if (head == nullptr) {
        head = List::make(r->read_expression(), nil());
        current = head;
      } else {
        List *next = cons(r->read_expression(), nil());
        current->set_cdr(next);
        current = next;
      }
    }

    return head;
  }
};

class QuoteParser : public Parser {
public:
  QuoteParser() { quote = Symbol::intern("quote"); }
  virtual bool relevant(const std::string &token) override {
    return token == S_SQUOTE;
  }

  virtual Object *parse(Token, Reader *r) override {
    return List::of({quote, r->read_expression()});
  }

private:
  Symbol *quote;
};
} // namespace

Reader::Reader(std::istream &in) : in{in} {
  parsers = {new NilParser, new NumberParser, new SymbolParser, new ListParser,
             new QuoteParser};
}

Reader::~Reader() {
  for (auto ptr : parsers) {
    delete ptr;
  }
}

Object *Reader::read_expression() {
  if (auto token = next_token(in); token.has_value()) {
    Parser &p = parser_for_token(token.value());
    return p.parse(token.value(), this);
  } else {
    return nil();
  }
}

Token Reader::get_token() {
  if (at_eof()) {
    throw std::runtime_error{"encountered EOF while parsing"};
  }
  if (auto token = next_token(in); token.has_value()) {
    return token.value();
  } else {
    throw std::runtime_error{"no token ready"};
  }
}

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
