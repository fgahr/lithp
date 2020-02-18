#ifndef _LITHP_READER_READER_H_
#define _LITHP_READER_READER_H_

#include <string>
#include <vector>

#include <object.hpp>
#include <util/stream.hpp>

namespace lithp::reader {
using TokenStream = util::Stream<std::string>;
using ObjectStream = util::Stream<Object *>;

class Parser;

class Reader {
public:
  Reader();
  ~Reader();
  Reader(const Reader &other) = delete;
  Object *parse_next(TokenStream &tokens);
  std::vector<Object *> read(std::vector<std::string> tokens);

private:
  Parser *parser_for_token(const std::string &token);
  std::vector<Parser *> parsers;
};
} // namespace lithp::reader

#endif // _LITHP_READER_READER_H_
