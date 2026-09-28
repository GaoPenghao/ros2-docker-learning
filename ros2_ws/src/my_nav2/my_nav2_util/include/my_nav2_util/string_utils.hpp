#ifndef MY_NAV2_UTIL__STRING_UTILS_HPP_
#define MY_NAV2_UTIL__STRING_UTILS_HPP_

#include <string>
#include <vector>

namespace my_nav2::util {

typedef std::vector<std::string> Tokens;

/**
 * @brief Remove leading slash from a topic name
 * @param in String of topic in
 * @return String out without slash
 */
std::string strip_leading_slash(const std::string& in);

/**
 * @brief Split a string at the delimiters
 * @param in String to split
 * @param delimiter criteria
 * @return Tokens
 */
Tokens split(const std::string& tokenstring, char delimiter);

}  // namespace my_nav2::util

#endif  // MY_NAV2_UTIL__STRING_UTILS_HPP_
