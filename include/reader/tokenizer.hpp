#ifndef _LITHP_READER_TOKENIZER_H_
#define _LITHP_READER_TOKENIZER_H_

#include <istream>
#include <string>
#include <vector>

namespace lithp::reader {
std::vector<std::string> tokenize(std::istream &in);
} // namespace lithp::reader

#endif // _LITHP_READER_TOKENIZER_H_
