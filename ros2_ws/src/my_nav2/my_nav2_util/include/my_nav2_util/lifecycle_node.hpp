#ifndef MY_NAV2_UTIL__LIFECYCLE_NODE_HPP_
#define MY_NAV2_UTIL__LIFECYCLE_NODE_HPP_

#include <memory>
#include <string>
#include <thread>

#include "bond/msg/constants.hpp"
#include "bondcpp/bond.hpp"
#include "my_nav2_util/node_thread.hpp"
#include "rcl_interfaces/msg/parameter_descriptor.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

namespace my_nav2::util {

using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class LifecycleNode : public rclcpp_lifecycle::LifecycleNode {
 public:
  /**
   * @brief A lifecycle node constructor
   * @param node_name Name for the node
   * @param namespace Namespace for the node, if any
   * @param options Node options
   */
  LifecycleNode(const std::string& node_name, const std::string& ns = "",
                const rclcpp::NodeOptions& options = rclcpp::NodeOptions());
  virtual ~LifecycleNode();

  typedef struct {
    double from_value;
    double to_value;
    double step;
  } floating_point_range;

  typedef struct {
    int from_value;
    int to_value;
    int step;
  } integer_range;

  /**
   * @brief Declare a parameter that has no integer or floating point range
   * constraints
   * @param name Name of parameter
   * @param default_value Default node value to add
   * @param description Node description
   * @param additional_constraints Any additional constraints on the parameters
   * to list
   * @param read_only Whether this param should be considered read only
   */
  void add_parameter(const std::string& name,
                     const rclcpp::ParameterValue& default_value,
                     const std::string& description = "",
                     const std::string& additional_constraints = "",
                     bool read_only = false) {
    auto descriptor = rcl_interfaces::msg::ParameterDescriptor();

    descriptor.name = name;
    descriptor.description = description;
    descriptor.additional_constraints = additional_constraints;
    descriptor.read_only = read_only;

    declare_parameter(descriptor.name, default_value, descriptor);
  }

  /**
   * @brief Declare a parameter that has a floating point range constraint
   * @param name Name of parameter
   * @param default_value Default node value to add
   * @param fp_range floating point range
   * @param description Node description
   * @param additional_constraints Any additional constraints on the parameters
   * to list
   * @param read_only Whether this param should be considered read only
   */
  void add_parameter(const std::string& name,
                     const rclcpp::ParameterValue& default_value,
                     const floating_point_range fp_range,
                     const std::string& description = "",
                     const std::string& additional_constraints = "",
                     bool read_only = false) {
    auto descriptor = rcl_interfaces::msg::ParameterDescriptor();

    descriptor.name = name;
    descriptor.description = description;
    descriptor.additional_constraints = additional_constraints;
    descriptor.read_only = read_only;
    descriptor.floating_point_range.resize(1);
    descriptor.floating_point_range[0].from_value = fp_range.from_value;
    descriptor.floating_point_range[0].to_value = fp_range.to_value;
    descriptor.floating_point_range[0].step = fp_range.step;

    declare_parameter(descriptor.name, default_value, descriptor);
  }

  /**
   * @brief Declare a parameter that has an integer range constraint
   * @param name Name of parameter
   * @param default_value Default node value to add
   * @param int_range Integer range
   * @param description Node description
   * @param additional_constraints Any additional constraints on the parameters
   * to list
   * @param read_only Whether this param should be considered read only
   */
  void add_parameter(const std::string& name,
                     const rclcpp::ParameterValue& default_value,
                     const integer_range int_range,
                     const std::string& description = "",
                     const std::string& additional_constraints = "",
                     bool read_only = false) {
    auto descriptor = rcl_interfaces::msg::ParameterDescriptor();

    descriptor.name = name;
    descriptor.description = description;
    descriptor.additional_constraints = additional_constraints;
    descriptor.read_only = read_only;
    descriptor.integer_range.resize(1);
    descriptor.integer_range[0].from_value = int_range.from_value;
    descriptor.integer_range[0].to_value = int_range.to_value;
    descriptor.integer_range[0].step = int_range.step;

    declare_parameter(descriptor.name, default_value, descriptor);
  }

  /**
   * @brief Get a shared pointer of this
   */
  std::shared_ptr<my_nav2::util::LifecycleNode> shared_from_this() {
    return std::static_pointer_cast<my_nav2::util::LifecycleNode>(
        rclcpp_lifecycle::LifecycleNode::shared_from_this());
  }

  /**
   * @brief Abstracted on_error state transition callback, since unimplemented
   * as of 2020 in the managed ROS2 node state machine
   * @param state State prior to error transition
   * @return Return type for success or failed transition to error state
   */
  my_nav2::util::CallbackReturn on_error(
      const rclcpp_lifecycle::State& /*state*/) {
    RCLCPP_FATAL(get_logger(),
                 "Lifecycle node %s does not have error state implemented",
                 get_name());
    return my_nav2::util::CallbackReturn::SUCCESS;
  }

  /**
   * @brief Automatically configure and active the node
   */
  void autostart();

  /**
   * @brief Perform preshutdown activities before our Context is shutdown.
   * Note that this is related to our Context's shutdown sequence, not the
   * lifecycle node state machine.
   */
  virtual void on_rcl_preshutdown();

  /**
   * @brief Create bond connection to lifecycle manager
   */
  void createBond();

  /**
   * @brief Destroy bond connection to lifecycle manager
   */
  void destroyBond();

 protected:
  /**
   * @brief Print notifications for lifecycle node
   */
  void printLifecycleNodeNotification();

  /**
   * @brief Register our preshutdown callback for this Node's rcl Context.
   * The callback fires before this Node's Context is shutdown.
   * Note this is not directly related to the lifecycle state machine.
   */
  void register_rcl_preshutdown_callback();

  /**
   * @brief Run some common cleanup steps shared between rcl preshutdown and
   * destruction.
   */
  void runCleanups();

 protected:
  std::unique_ptr<rclcpp::PreShutdownCallbackHandle> rcl_preshutdown_cb_handle_{
      nullptr};

  // Connection to tell that server is still up
  std::shared_ptr<bond::Bond> bond_{nullptr};
  double bond_heartbeat_period_;
  rclcpp::TimerBase::SharedPtr autostart_timer_;
};

}  // namespace my_nav2::util

#endif  // MY_NAV2_UTIL__LIFECYCLE_NODE_HPP_
