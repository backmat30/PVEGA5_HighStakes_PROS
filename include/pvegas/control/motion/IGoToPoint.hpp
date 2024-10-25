#ifndef __I_GO_TO_POINT_HPP__
#define __I_GO_TO_POINT_HPP__

#include <memory>

#include "pvegas/control/Point.hpp"
#include "pvegas/robot/Robot.hpp"

namespace pvegas {
namespace control {
namespace motion {
class IGoToPoint {
 public:
  // destroyer
  virtual ~IGoToPoint() = default;

  // initialize the control
  virtual void init() = 0;

  // run the control
  virtual void run() = 0;

  // pause the control
  virtual void pause() = 0;

  // resume the control
  virtual void resume() = 0;

  // tell the robot to go to a given point
  virtual void goToPoint(const std::shared_ptr<pvegas::robot::Robot>& robot, double velocity,
                         Point point) = 0;

  // set the max velocity to move at
  virtual void setVelocity(double velocity) = 0;

  // determines if the robot has reached the target
  virtual bool targetReached() = 0;
};
}  // namespace motion
}  // namespace control
}  // namespace pvegas
#endif