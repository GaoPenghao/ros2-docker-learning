#include <iostream>
#include <string>

// 前向声明，绕开 ROS 头文件
namespace my_nav2::util {
std::string time_to_string(size_t len);
std::string sanitize_node_name(const std::string& potential_node_name);
}  // namespace my_nav2::util

int main() {
  std::cout << "len=8:  '" << my_nav2::util::time_to_string(8) << "'"
            << std::endl;
  std::cout << "len=4:  '" << my_nav2::util::time_to_string(4) << "'"
            << std::endl;
  std::cout << "len=20: '" << my_nav2::util::time_to_string(20) << "'"
            << std::endl;

  std::cout << "sanitize('/foo.bar baz'):     '"
            << my_nav2::util::sanitize_node_name("/foo.bar baz") << "'"
            << std::endl;
  std::cout << "sanitize('node-name-123'):    '"
            << my_nav2::util::sanitize_node_name("node-name-123") << "'"
            << std::endl;
  std::cout << "sanitize('already_valid'):    '"
            << my_nav2::util::sanitize_node_name("already_valid") << "'"
            << std::endl;

  return 0;
}
