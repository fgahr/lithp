#ifndef _LITHP_UTIL_STRING_H_
#define _LITHP_UTIL_STRING_H_

#include <string>

namespace lithp::util::str {
std::string upcase(std::string s);
std::string downcase(std::string s);
std::string invert_case(std::string s);
} // namespace lithp::util::str

#endif // _LITHP_UTIL_STRING_H_
