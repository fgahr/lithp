#include <regex>
#include <string>

#include <lithp.hpp>
#include <reader/reader.hpp>
#include <reader/tokenizer.hpp>
#define LITHP_READER_STRING_TOKENS
#include <reader/tokens.hpp>
#undef LITHP_READER_STRING_TOKENS

namespace lithp::reader {

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
  virtual bool relevant(const Token &token) override { return token == LPAREN; }
  virtual List *parse(Token, TokenStream &tokens) override {
    Token token;
    List *head = nullptr;
    List *current = nullptr;
    // FIXME: Will crash with not-helpful error message if parens are unbalanced
    while ((token = tokens.get()) != RPAREN) {
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
    return token == SQUOTE;
  }

  virtual Object *parse(Token, TokenStream &tokens) override {
    return List::of({quote, reader->parse_next(tokens)});
  }

private:
  Symbol *quote;
  Reader *reader;
};
} // namespace

Reader::Reader(std::istream &in) {
  parsers = {new NilParser(), new NumberParser(), new SymbolParser(),
             new ListParser{this}, new QuoteParser{this}};
  tokens = TokenStream::of(tokenize(in));
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

Object *Reader::read_expression() {
  return parse_next(tokens);
}

Parser &Reader::parser_for_token(const std::string &token) {
  for (auto parser : parsers) {
    if (parser->relevant(token)) {
      return *parser;
    }
  }
  throw std::runtime_error{"token not recognized: " + token};
}
} // namespace lithp::reader
