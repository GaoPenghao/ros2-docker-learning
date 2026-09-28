#include "my_nav2_util/string_utils.hpp"

namespace my_nav2::util {

std::string strip_leading_slash(const std::string& in) {
  std::string out = in;

  if ((!in.empty()) && (in[0] == '/')) {
    out.erase(0, 1);
  }

  return out;
}

Tokens split(const std::string& tokenstring, char delimiter) {
  Tokens tokens;

  size_t current_pos = 0;
  size_t pos = 0;
  while ((pos = tokenstring.find(delimiter, current_pos)) !=
         std::string::npos) {
    tokens.emplace_back(tokenstring.substr(current_pos, pos - current_pos));
    current_pos = pos + 1;
  }
  tokens.emplace_back(tokenstring.substr(current_pos));

  return tokens;
}

}  // namespace my_nav2::util
