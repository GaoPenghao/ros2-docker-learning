#include "my_nav2_util/node_utils.hpp"

#include <sched.h>

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <chrono>
#include <cstring>

namespace my_nav2::util {

std::string sanitize_node_name(const std::string& potential_node_name) {
  std::string node_name(potential_node_name);
  // read this as `replace` characters in `node_name` `if` not alphanumeric.
  // replace with '_'
  std::replace_if(
      std::begin(node_name), std::end(node_name),
      [](auto c) { return !std::isalnum(c); }, '_');
  return node_name;
}

std::string add_namespaces(const std::string& top_ns,
                           const std::string& sub_ns) {
  if (!top_ns.empty() && top_ns.back() == '/') {
    if (top_ns.front() == '/') {
      return top_ns + sub_ns;
    } else {
      return "/" + top_ns + sub_ns;
    }
  }

  return top_ns + "/" + sub_ns;
}

std::string generate_internal_node_name(const std::string& prefix) {
  return sanitize_node_name(prefix) + "_" + time_to_string(8);
}

rclcpp::Node::SharedPtr generate_internal_node(const std::string& prefix) {
  auto options =
      rclcpp::NodeOptions()
          .start_parameter_services(false)
          .start_parameter_event_publisher(false)
          .arguments({"--ros-args", "-r",
                      "__node:=" + generate_internal_node_name(prefix), "--"});
  return rclcpp::Node::make_shared("_", options);
}

std::string time_to_string(size_t len) {
  std::string output(len, '0');  // prefill the string with zeros
  auto timepoint = std::chrono::high_resolution_clock::now();
  auto timecount = timepoint.time_since_epoch().count();
  auto timestring = std::to_string(timecount);
  if (timestring.length() >= len) {
    // if `output` is shorter, just copy in the end of `timestring`
    output.replace(0, len, timestring, timestring.length() - len, len);
  } else {
    // if `timestring` is shorter, put it at the end of `output`
    output.replace(len - timestring.length(), timestring.length(), timestring,
                   0, timestring.length());
  }

  return output;
}

void setSoftRealTimePriority() {
  sched_param sch;
  sch.sched_priority = 49;
  if (sched_setscheduler(0, SCHED_FIFO, &sch) == -1) {
    std::string errmsg(
        "Cannot set as real-time thread. Users must set: <username> hard "
        "rtprio 99 and <username> soft rtprio 99 in /etc/security/limits.conf "
        "to enable realtime prioritization! Error: ");
    throw std::runtime_error(errmsg + std::strerror(errno));
  }
}

}  // namespace my_nav2::util
