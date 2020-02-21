#include <regex>
#include <string>

#include <lithp.hpp>
#include <reader/reader.hpp>
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

class NilParser : public Parser {
public:
  virtual bool relevant(const Token &token) override { return token == "nil"; }
  virtual Object *parse(Token first, TokenStream &rest) override {
    return nil();
  }
};

class NumberParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    std::regex number{"-?[0-9]+"};
    return std::regex_match(token, number);
  }
  virtual Number *parse(Token first, TokenStream &rest) override {
    return Number::make(std::stol(first));
  }
};

class SymbolParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    return Symbol::is_valid(token);
  }
  virtual Symbol *parse(Token first, TokenStream &tokens) override {
    return Symbol::intern(first);
  }
};

class ListParser : public Parser {
public:
  ListParser(Reader *reader) : reader{reader} {}
  virtual bool relevant(const std::string &token) override {
    return token == LPAREN;
  }
  virtual List *parse(Token first, TokenStream &tokens) override {
    Token token;
    List *head = nullptr;
    List *current = nullptr;
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

  virtual Object *parse(Token first, TokenStream &tokens) override {
    return List::of({quote, reader->parse_next(tokens)});
  }

private:
  Symbol *quote;
  Reader *reader;
};

Reader::Reader() {
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

std::vector<Object *> Reader::read(std::vector<std::string> tokens) {
  std::vector<Object *> program;
  TokenStream tstream = TokenStream::of(tokens);

  while (tstream.has_more()) {
    program.push_back(parse_next(tstream));
  }

  return program;
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
