#ifndef __INERTIAL_POSITION_TRACKER_HPP__
#define __INERTIAL_POSITION_TRACKER_HPP__

#include <cmath>
#include <memory>

#include "pvegas/io/IDistanceTracker.hpp"
#include "pvegas/io/IInertialSensor.hpp"
#include "pvegas/robot/subsystems/odometry/IPositionTracker.hpp"
#include "pvegas/rtos/IClock.hpp"
#include "pvegas/rtos/IDelayer.hpp"
#include "pvegas/rtos/IMutex.hpp"
#include "pvegas/rtos/ITask.hpp"

namespace pvegas {
namespace robot {
namespace subsystems {
namespace odometry {
class InertialPositionTracker : public IPositionTracker {
 private:
  // the delay between task loops
  static constexpr uint8_t TASK_DELAY{20};

  // conversion factor between milliseconds and seconds
  static constexpr double MS_TO_SECONDS{1.0 / 1000.0};

  // task loop to update provided tracker
  static void taskLoop(void* params);

  // clock object
  std::unique_ptr<pvegas::rtos::IClock> m_clock{};

  // task delayer
  std::unique_ptr<pvegas::rtos::IDelayer> m_delayer{};

  // mutex
  std::unique_ptr<pvegas::rtos::IMutex> m_mutex{};

  // task for updates
  std::unique_ptr<pvegas::rtos::ITask> m_task{};

  // inertial sensor to track rotation
  std::unique_ptr<pvegas::io::IInertialSensor> m_inertial_sensor{};

  // distance tracker on the left side of the robot
  std::unique_ptr<pvegas::io::IDistanceTracker> m_left_distance_tracker{};

  // the offset, from the left side of the robot, of the left distance tracker
  double m_left_distance_tracker_offset{};

  // distance tracker on the right side of the robot
  std::unique_ptr<pvegas::io::IDistanceTracker> m_right_distance_tracker{};

  // the offset, from the left side of the robot, of the right distance tracker
  double m_right_distance_tracker_offset{};

  // the position on the field
  Position m_position{};

  // the heading offset
  double m_heading_offset{};

  // the latest heading value
  double last_heading{};

  // the latest distance recorded on the left tracking wheel
  double last_left_distance{};

  // the latest distance recorded on the right tracking wheel
  double last_right_distance{};

  // the system time of the latest update
  uint8_t last_time{};

  // run all updates in the system
  void taskUpdate();

  // update the position of the system
  void updatePosition();

 public:
  // initialize the system
  void init() override;

  // runs the system
  void run() override;

  // sets the position of the system
  void setPosition(Position Position) override;

  // gets the position of the system
  Position getPosition() override;

  // set the x position
  void setX(double x) override;

  // set the y position
  void setY(double y) override;

  // set the angular offset of the system
  void setTheta(double theta);

  // set the clock
  void setClock(std::unique_ptr<pvegas::rtos::IClock>& clock);

  // set the delayer
  void setDelayer(std::unique_ptr<pvegas::rtos::IDelayer>& delayer);

  // set the mutex
  void setMutex(std::unique_ptr<pvegas::rtos::IMutex>& mutex);

  // set the task
  void setTask(std::unique_ptr<pvegas::rtos::ITask>& task);

  // set the inertial sensor
  void setInertialSensor(
      std::unique_ptr<pvegas::io::IInertialSensor>& inertial_sensor);

  // set the left distance tracker
  void setLeftDistanceTracker(
      std::unique_ptr<pvegas::io::IDistanceTracker>& left_distance_tracker);

  // set the offset of the left distance tracker
  void setLeftDIstanceTrackerOffset(double left_distance_tracker_offset);

  // set the right distance tracker
  void setRightDistanceTracker(
      std::unique_ptr<pvegas::io::IDistanceTracker>& right_distance_tracker);

  // set the offset of the right distance tracker
  void setRightDistanceTrackerOffset(double right_distance_tracker_offset);
};
}  // namespace odometry
}  // namespace subsystems
}  // namespace robot
}  // namespace pvegas
#endif