#ifndef __INTAKE_OPERATOR_HPP__
#define __INTAKE_OPERATOR_HPP__

#include <memory>

#include "driftless/io/IController.hpp"
#include "driftless/op_control/EControllerDigital.hpp"
#include "driftless/op_control/intake/EIntakeControlMode.hpp"
#include "driftless/profiles/IProfile.hpp"
#include "driftless/robot/Robot.hpp"

namespace driftless {
namespace op_control {
namespace intake {
class IntakeOperator {
 private:
  // name of the intake subsystem
  static constexpr char INTAKE_SUBSYSTEM_NAME[]{"INTAKE"};

  // name of the command to spin the intake motors
  static constexpr char SPIN_COMMAND_NAME[]{"SPIN"};

  // name of the command to set the height of the intake
  static constexpr char SET_HEIGHT_COMMAND_NAME[]{"SET HEIGHT"};

  // name of the state of the intake height
  static constexpr char GET_HEIGHT_STATE_NAME[]{"GET HEIGHT"};

  // the controller used
  std::shared_ptr<driftless::io::IController> m_controller{};

  // the robot being controlled
  std::shared_ptr<driftless::robot::Robot> m_robot{};

  // update the intake height with split toggle
  void updateSplitToggle(EControllerDigital up, EControllerDigital down);

  // update the intake height with single toggle
  void updateSingleToggle(EControllerDigital toggle);

  // update the intake height with hold up
  void updateHoldUp(EControllerDigital up);

  // update the intake spinner
  void updateSpinner(EControllerDigital spin, EControllerDigital reverse);

 public:
  // create a new intake operator object
  IntakeOperator(const std::shared_ptr<driftless::io::IController>& controller,
                 const std::shared_ptr<driftless::robot::Robot>& robot);

  // update the intake
  void update(const std::unique_ptr<driftless::profiles::IProfile>& profile);
};
}  // namespace intake
}  // namespace op_control
}  // namespace driftless
#endif