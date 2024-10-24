#ifndef __PID_DRIVE_STRAIGHT_HPP__
#define __PID_DRIVE_STRAIGHT_HPP__

#include <cmath>
#include <memory>

#include "pvegas/control/PID.hpp"
#include "pvegas/control/Point.hpp"
#include "pvegas/control/motion/IDriveStraight.hpp"
#include "pvegas/robot/subsystems/drivetrain/Velocity.hpp"
#include "pvegas/robot/subsystems/odometry/Position.hpp"
#include "pvegas/rtos/IDelayer.hpp"
#include "pvegas/rtos/IMutex.hpp"
#include "pvegas/rtos/ITask.hpp"
#include "pvegas/utils/UtilityFunctions.hpp"

namespace pvegas {
namespace control {
namespace motion {
class PIDDriveStraight : public pvegas::control::motion::IDriveStraight {
 private:
  // delay on the task loop
  static constexpr uint8_t TASK_DELAY{10};

  // task loop to run background updates
  static void taskLoop(void* params);

  // the delayer
  std::unique_ptr<pvegas::rtos::IDelayer> m_delayer{};

  // the mutex
  std::unique_ptr<pvegas::rtos::IMutex> m_mutex{};

  // the task
  std::unique_ptr<pvegas::rtos::ITask> m_task{};

  // the robot being controlled
  std::shared_ptr<pvegas::robot::Robot> m_robot{};

  // the linear pid controller
  PID m_linear_pid{};

  // the rotational pid controller
  PID m_rotational_pid{};

  // the tolerance for being at the target
  double m_target_tolerance{};

  // the max velocity for being considered at the target
  double m_target_velocity{};

  // the limit to the velocity output
  double m_max_velocity{};

  // the starting point
  Point m_starting_point{};

  // the target distance
  double m_target_distance{};

  // the target angle
  double m_target_angle{};

  // whether the robot has reached its current target
  bool target_reached{true};

  // whether the control has been paused
  bool paused{};

  // sets the velocity of the drivetrain
  void setDriveVelocity(double left, double right);

  // get the position of the robot
  robot::subsystems::odometry::Position getPosition();

  // get the velocity of the robot
  double getVelocity();

  // update the control velocity
  void updateVelocity(double distance, double theta);

  // run all updates
  void taskUpdate();

 public:
  // initialize the control
  void init() override;

  // run the control
  void run() override;

  // pause the control
  void pause() override;

  // resume the control
  void resume() override;

  // tell the robot to drive straight for a given distance in inches
  void driveStraight(std::shared_ptr<pvegas::robot::Robot>& robot,
                     double velocity, double distance, double theta) override;

  // set the velocity to run the control at
  void setVelocity(double velocity) override;

  // return if the robot has reached the target
  bool targetReached() override;

  // set the delayer
  void setDelayer(const std::unique_ptr<pvegas::rtos::IDelayer>& delayer);

  // set the mutex
  void setMutex(std::unique_ptr<pvegas::rtos::IMutex>& mutex);

  // set the task
  void setTask(std::unique_ptr<pvegas::rtos::ITask>& task);

  // set the linear pid controller
  void setLinearPID(PID linear_pid);

  // set the rotational pid controller
  void setRotationalPID(PID rotational_pid);

  // sets the target tolerance
  void setTargetTolerance(double target_tolerance);

  // sets the target velocity
  void setTargetVelocity(double target_velocity);
};
}  // namespace motion
}  // namespace control
}  // namespace pvegas
#endif