#ifndef __PID_ARM_MOTION_HPP__
#define __PID_ARM_MOTION_HPP__

#include <cmath>
#include <memory>

#include "pvegas/control/PID.hpp"
#include "pvegas/hal/MotorGroup.hpp"
#include "pvegas/io/IPotentiometer.hpp"
#include "pvegas/io/IRotationSensor.hpp"
#include "pvegas/robot/subsystems/arm/IArmMotion.hpp"
#include "pvegas/rtos/IDelayer.hpp"
#include "pvegas/rtos/IMutex.hpp"
#include "pvegas/rtos/ITask.hpp"

namespace pvegas {
namespace robot {
namespace subsystems {
namespace arm {
class PIDArmMotion : public IArmMotion {
 private:
  enum class EState {
    NEUTRAL,
    LOAD,
    READY,
    SCORE,
    NEUTRAL_MOTION,
    LOAD_MOTION,
    READY_MOTION,
    SCORE_MOTION
  };
  // delay between task updates
  static constexpr uint8_t TASK_DELAY{10};

  // the conversion factor from motor rotations to arm rotations
  static constexpr double MOTOR_TO_ARM_ROTATIONS{1.0 / 4.0};

  // the conversion factor from potentiometer or rotation sensor rotations to
  // arm rotations
  static constexpr double SENSOR_TO_ARM_ROTATIONS{3.0 / 4.0};

  // runs task updates on loop
  static void taskLoop(void* params);

  // the delayer used by the subsystem
  std::unique_ptr<pvegas::rtos::IDelayer> m_delayer{};

  // the mutex used for task-related functions
  std::unique_ptr<pvegas::rtos::IMutex> m_mutex{};

  // the task used by the subsystem
  std::unique_ptr<pvegas::rtos::ITask> m_task{};

  // rotation sensor for the arm rotation
  std::unique_ptr<pvegas::io::IRotationSensor> m_rotation_sensor{};

  // potentiometer for arm rotation
  std::unique_ptr<pvegas::io::IPotentiometer> m_potentiometer{};

  // the motors used to rotate the arm
  pvegas::hal::MotorGroup m_rotation_motors{};

  // the motors used to control the length of the arm
  pvegas::hal::MotorGroup m_linear_motors{};

  // the rotational PID controller
  pvegas::control::PID m_rotational_pid{};

  // the linear PID controller
  pvegas::control::PID m_linear_pid{};

  // the current position of the arm
  EState state{EState::LOAD};

  // the rotational position when neutral
  double m_rotational_neutral_position{};

  // the rotational position when loading
  double m_rotational_load_position{};

  // the rotational position when ready
  double m_rotational_ready_position{};

  // the rotational position when scoring
  double m_rotational_score_position{};

  // the rotational position tolerance
  double m_rotational_tolerance{};

  // the target rotational position
  double rotational_target_position{};

  // the linear position when neutral
  double m_linear_neutral_position{};

  // the linear position when loading
  double m_linear_load_position{};

  // the linear position when ready
  double m_linear_ready_position{};

  // the linear position when scoring
  double m_linear_score_position{};

  // the linear position tolerance
  double m_linear_tolerance{};

  // the target linear position
  double linear_target_position{};

  // update all task-related features
  void taskUpdate();

  // update the state of the arm
  void updateState();

  // update the position of the arm motors
  void updatePosition();

  // gets the rotational position
  double getRotationalPosition();

  // gets the linear position
  double getLinearPosition();

 public:
  // initialize the arm motion control
  void init() override;

  // run the arm motion control
  void run() override;

  // goes to the neutral position
  void goNeutral() override;

  // goes to the loading position
  void goLoad() override;

  // goes to the ready position
  void goReady() override;

  // goes to the score position
  void goScore() override;

  // determines if the arm is in the neutral position
  bool isAtNeutral() override;

  // determines if the arm is going to the neutral position
  bool isGoingNeutral() override;

  // determines if the arm is in the loading position
  bool isAtLoad() override;

  // determines if the arm is going to the loading position
  bool isGoingLoad() override;

  // determines if the arm is in the ready position
  bool isAtReady() override;

  // determines if the arm is going to the ready position
  bool isGoingReady() override;

  // determines if the arm is in the score position
  bool isAtScore() override;

  // determines if the arm is going to the score position
  bool isGoingScore() override;

  // sets the delayer
  void setDelayer(const std::unique_ptr<pvegas::rtos::IDelayer>& delayer);

  // sets the mutex
  void setMutex(std::unique_ptr<pvegas::rtos::IMutex>& mutex);

  // sets the task
  void setTask(std::unique_ptr<pvegas::rtos::ITask>& task);

  // sets the rotation sensor
  void setRotationSensor(
      std::unique_ptr<pvegas::io::IRotationSensor>& rotation_sensor);

  // sets the potentiometer
  void setPotentiometer(
      std::unique_ptr<pvegas::io::IPotentiometer>& potentiometer);

  // sets the rotation motors
  void setRotationMotors(pvegas::hal::MotorGroup& rotation_motors);

  // sets the linear motors
  void setLinearMotors(pvegas::hal::MotorGroup& linear_motors);

  // sets the rotational PID controller
  void setRotationalPID(pvegas::control::PID rotational_pid);

  // sets the linear PID controller
  void setLinearPID(pvegas::control::PID linear_pid);

  // sets the rotational neutral position
  void setRotationalNeutralPosition(double rotational_neutral_position);

  // sets the rotational loading position
  void setRotationalLoadPosition(double rotational_load_position);

  // sets the rotational ready position
  void setRotationalReadyPosition(double rotational_ready_position);

  // sets the rotational score position
  void setRotationalScorePosition(double rotational_score_position);

  // sets the rotational position tolerance
  void setRotationalTolerance(double rotational_tolerance);

  // sets the linear neutral position
  void setLinearNeutralPosition(double linear_neutral_position);

  // sets the linear loading position
  void setLinearLoadPosition(double linear_load_position);

  // sets the linear ready position
  void setLinearReadyPosition(double linear_ready_position);

  // sets the linear score position
  void setLinearScorePosition(double linear_score_position);

  // sets the linear position tolerance
  void setLinearTolerance(double linear_tolerance);
};
}  // namespace arm
}  // namespace subsystems
}  // namespace robot
}  // namespace pvegas
#endif