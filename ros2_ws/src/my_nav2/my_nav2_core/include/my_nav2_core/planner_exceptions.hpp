#ifndef MY_NAV2_CORE__PLANNER_EXCEPTIONS_HPP_
#define MY_NAV2_CORE__PLANNER_EXCEPTIONS_HPP_

#include <memory>
#include <stdexcept>
#include <string>

namespace my_nav2::core {

class PlannerException : public std::runtime_error {
 public:
  explicit PlannerException(const std::string& description)
      : std::runtime_error(description) {}
};

class InvalidPlanner : public PlannerException {
 public:
  explicit InvalidPlanner(const std::string& description)
      : PlannerException(description) {}
};

class StartOccupied : public PlannerException {
 public:
  explicit StartOccupied(const std::string& description)
      : PlannerException(description) {}
};

class GoalOccupied : public PlannerException {
 public:
  explicit GoalOccupied(const std::string& description)
      : PlannerException(description) {}
};

class StartOutsideMapBounds : public PlannerException {
 public:
  explicit StartOutsideMapBounds(const std::string& description)
      : PlannerException(description) {}
};

class GoalOutsideMapBounds : public PlannerException {
 public:
  explicit GoalOutsideMapBounds(const std::string& description)
      : PlannerException(description) {}
};

class NoValidPathCouldBeFound : public PlannerException {
 public:
  explicit NoValidPathCouldBeFound(const std::string& description)
      : PlannerException(description) {}
};

class PlannerTimedOut : public PlannerException {
 public:
  explicit PlannerTimedOut(const std::string& description)
      : PlannerException(description) {}
};

class PlannerTFError : public PlannerException {
 public:
  explicit PlannerTFError(const std::string& description)
      : PlannerException(description) {}
};

class NoViapointsGiven : public PlannerException {
 public:
  explicit NoViapointsGiven(const std::string& description)
      : PlannerException(description) {}
};

class PlannerCancelled : public PlannerException {
 public:
  explicit PlannerCancelled(const std::string& description)
      : PlannerException(description) {}
};

}  // namespace my_nav2::core

#endif  // MY_NAV2_CORE__PLANNER_EXCEPTIONS_HPP_
