#include <iostream>

#include "my_nav2_util/array_parser.hpp"

int main() {
  std::string error;

  // 正常情况
  auto v1 = my_nav2::util::parseVVF("[[1.0, 2.0], [3.3, 4.4, 5.5]]", error);
  std::cout << "Test 1 error: '" << error << "'" << std::endl;
  for (const auto& row : v1) {
    for (auto val : row) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }

  // 深度过深
  auto v2 = my_nav2::util::parseVVF("[[[1.0]]]", error);
  std::cout << "Test 2 error: '" << error << "'" << std::endl;

  // 未闭合
  auto v3 = my_nav2::util::parseVVF("[[1.0, 2.0]", error);
  std::cout << "Test 3 error: '" << error << "'" << std::endl;

  // 深度不够
  auto v4 = my_nav2::util::parseVVF("[1.0, 2.0]", error);
  std::cout << "Test 4 error: '" << error << "'" << std::endl;

  return 0;
}
