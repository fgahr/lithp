#include <regex>
#include <string>

#include <lithp.hpp>
#include <reader/reader.hpp>
#define LITHP_READER_STRING_TOKENS
#include <reader/tokens.hpp>

namespace lithp::reader {
using Token = std::string;

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
    return Nil::nil();
  }
};

class NumberParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    std::regex number{"-?[0-9]+"};
    return std::regex_match(token, number);
  }
  virtual Object *parse(Token first, TokenStream &rest) override {
    return Number::make(std::stol(first));
  }
};

class SymbolParser : public Parser {
public:
  virtual bool relevant(const Token &token) override {
    return Symbol::is_valid(token);
  }
  virtual Object *parse(Token first, TokenStream &tokens) override {
    return Symbol::intern(first);
  }
};

namespace {
struct Sexp : public Object {
  virtual Type type() override {
    // Will not be the type of any other object while parsing and so can be used
    // to distinguish these objects from all others. Introducing a global type
    // for this purpose could be irritating in other places.
    return Type::BrokenHeart;
  }
  virtual Object *eval(Environment &env) override {
    // TODO: IMPLEMENT!
    // Needs to do dispatch based on head, becoming e.g.
    // a function call or a special form.
    throw std::logic_error{"NOT YET IMPLEMENTED!"};
  }
  virtual void repr(std::ostream &out) override {
    throw std::logic_error{"attempting to print raw sexp"};
  }
  virtual RefStream refs(void) override { return RefStream::empty(); }
  virtual Object *copy_to(void *mem) override {
    throw std::logic_error{"attempting to copy raw sexp"};
  }
  static bool is_instance(Object *obj) {
    // See other comment
    LITHP_CHECK_TYPE(obj, BrokenHeart);
  }
  static Sexp *cast(Object *obj) {
    if (obj == nullptr || obj->type() != Type::BrokenHeart) {
      throw std::logic_error{"invalid cast to sexp"};
    }
    return static_cast<Sexp *>(obj);
  }
  Object *head() {
    if (objects.empty()) {
      throw std::logic_error{"empty sexp"};
    }
    return objects.front();
  }
  std::vector<Object *> tail() {
    std::vector<Object *> rest;
    // FIXME: Use std::copy?
    for (size_t i = 1; i < objects.size(); i++) {
      rest.push_back(objects.at(i));
    }
    return rest;
  }
  std::vector<Object *> objects;
};
} // namespace

class ListParser : public Parser {
public:
  ListParser(Reader *reader) : reader{reader} {}
  virtual bool relevant(const std::string &token) override {
    return token == LPAREN;
  }
  virtual Object *parse(Token first, TokenStream &tokens) override {
    // TODO: Rework!
    Token token;
    ConsCell *head = nullptr;
    ConsCell *current = nullptr;
    while ((token = tokens.get()) != RPAREN) {
      if (head == nullptr) {
        head = ConsCell::make(reader->parse_next(tokens), Nil::nil());
        current = head;
      } else {
        current->cdr = ConsCell::make(reader->parse_next(tokens), Nil::nil());
        current = ConsCell::cast(current->cdr);
      }
    }

    if (head) {
      return head;
    } else {
      // Loop body was never entered, empty list.
      return Nil::nil();
    }
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
    return token == LPAREN;
  }

  virtual Object *parse(Token first, TokenStream &tokens) override {
    return ConsCell::make(quote, reader->parse_next(tokens));
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

Object *Reader::parse_next(TokenStream &tokens) {
  Token token = tokens.get();
  Parser *p = parser_for_token(token);
  // TODO: Do additional checking here?
  return p->parse(token, tokens);
}

std::vector<Object *> Reader::read(std::vector<std::string> tokens) {
  std::vector<Object *> program;
  TokenStream tstream = TokenStream::of(tokens);

  while (tstream.has_more()) {
    program.push_back(parse_next(tstream));
  }

  return program;
}

Parser *Reader::parser_for_token(const std::string &token) {
  for (auto parser : parsers) {
    if (parser->relevant(token)) {
      return parser;
    }
  }
  throw std::runtime_error{"token not recognized: " + token};
}
} // namespace lithp::reader
