#include <algorithm>

#include <util/strings.hpp>

namespace lithp::util::str {
std::string upcase(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);
  return s;
}
std::string downcase(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
}

static char invert(char c) {
  if (std::isupper(c)) {
    return std::tolower(c);
  } else if (std::islower(c)) {
    return std::toupper(c);
  } else {
    return c;
  }
}
std::string invert_case(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), invert);
  return s;
}
} // namespace lithp::util::str
