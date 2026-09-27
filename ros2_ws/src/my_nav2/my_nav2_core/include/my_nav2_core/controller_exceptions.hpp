#ifndef MY_NAV2_CORE__CONTROLLER_EXCEPTIONS_HPP_
#define MY_NAV2_CORE__CONTROLLER_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

namespace my_nav2::core {

class ControllerException : public std::runtime_error {
 public:
  explicit ControllerException(const std::string& description)
      : std::runtime_error(description) {}
};

class InvalidController : public ControllerException {
 public:
  explicit InvalidController(const std::string& description)
      : ControllerException(description) {}
};

class ControllerTFError : public ControllerException {
 public:
  explicit ControllerTFError(const std::string& description)
      : ControllerException(description) {}
};

class FailedToMakeProgress : public ControllerException {
 public:
  explicit FailedToMakeProgress(const std::string& description)
      : ControllerException(description) {}
};

class PatienceExceeded : public ControllerException {
 public:
  explicit PatienceExceeded(const std::string& description)
      : ControllerException(description) {}
};

class InvalidPath : public ControllerException {
 public:
  explicit InvalidPath(const std::string& description)
      : ControllerException(description) {}
};

class NoValidControl : public ControllerException {
 public:
  explicit NoValidControl(const std::string& description)
      : ControllerException(description) {}
};

class ControllerTimedOut : public ControllerException {
 public:
  explicit ControllerTimedOut(const std::string& description)
      : ControllerException(description) {}
};

}  // namespace my_nav2::core

#endif  // MY_NAV2_CORE__CONTROLLER_EXCEPTIONS_HPP_
