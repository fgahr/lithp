#ifndef _LITHP_READER_READER_H_
#define _LITHP_READER_READER_H_

#include <string>
#include <vector>

#include <object.hpp>
#include <util/stream.hpp>

namespace lithp::reader {
using Token = std::string;
using TokenStream = util::Stream<std::string>;
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

private:
  Parser &parser_for_token(const std::string &token);
  TokenStream tokens = TokenStream::empty();
  std::vector<Parser *> parsers;
};
} // namespace lithp::reader

#endif // _LITHP_READER_READER_H_
