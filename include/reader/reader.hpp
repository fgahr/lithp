#ifndef _LITHP_READER_READER_H_
#define _LITHP_READER_READER_H_

#include <istream>

#include <object.hpp>
#include <util/stream.hpp>

namespace lithp::reader {
void init(std::istream &in);
bool done();
void exit();
Object *next_expr();
} // namespace lithp::reader

#endif // _LITHP_READER_READER_H_
