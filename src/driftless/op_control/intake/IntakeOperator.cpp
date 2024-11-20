#include "driftless/op_control/intake/IntakeOperator.hpp"

namespace driftless {
namespace op_control {
namespace intake {
void IntakeOperator::updateSplitToggle(EControllerDigital up,
                                       EControllerDigital down) {
  bool move_up{m_controller->getNewDigital(up)};
  bool move_down{m_controller->getNewDigital(down)};

  if (move_up && !move_down) {
    m_robot->sendCommand(INTAKE_SUBSYSTEM_NAME, SET_HEIGHT_COMMAND_NAME, true);
  } else if (!move_up && move_down) {
    m_robot->sendCommand(INTAKE_SUBSYSTEM_NAME, SET_HEIGHT_COMMAND_NAME, false);
  }
}

void IntakeOperator::updateSingleToggle(EControllerDigital toggle) {
  bool toggle_position{m_controller->getNewDigital(toggle)};

  if (toggle_position) {
    void* intake_state{
        m_robot->getState(INTAKE_SUBSYSTEM_NAME, GET_HEIGHT_STATE_NAME)};
    bool current_height{*static_cast<bool*>(intake_state)};

    m_robot->sendCommand(INTAKE_SUBSYSTEM_NAME, SET_HEIGHT_COMMAND_NAME,
                         !current_height);
  }
}

void IntakeOperator::updateHoldUp(EControllerDigital up) {
  bool move_up{m_controller->getDigital(up)};

  m_robot->sendCommand(INTAKE_SUBSYSTEM_NAME, SET_HEIGHT_COMMAND_NAME, move_up);
}

void IntakeOperator::updateSpinner(EControllerDigital spin, EControllerDigital reverse) {
  bool spin_motors{m_controller->getDigital(spin)};
  bool reverse_motors{m_controller->getDigital(reverse)};
  if (spin_motors && !reverse_motors) {
    m_robot->sendCommand(INTAKE_SUBSYSTEM_NAME, SPIN_COMMAND_NAME, 12.0);
  } else if (!spin_motors && reverse_motors) {
    m_robot->sendCommand(INTAKE_SUBSYSTEM_NAME, SPIN_COMMAND_NAME, -12.0);
  } else {
    m_robot->sendCommand(INTAKE_SUBSYSTEM_NAME, SPIN_COMMAND_NAME, 0.0);
  }
}

IntakeOperator::IntakeOperator(
    const std::shared_ptr<driftless::io::IController>& controller,
    const std::shared_ptr<driftless::robot::Robot>& robot)
    : m_controller{controller}, m_robot{robot} {}

void IntakeOperator::update(
    const std::unique_ptr<driftless::profiles::IProfile>& profile) {
  EControllerDigital toggle_up{
      profile->getDigitalControlMapping(EControl::INTAKE_RAISE)};
  EControllerDigital toggle_down{
      profile->getDigitalControlMapping(EControl::INTAKE_LOWER)};
  EControllerDigital toggle_states{
      profile->getDigitalControlMapping(EControl::INTAKE_TOGGLE_HEIGHT)};
  EControllerDigital hold_up{
      profile->getDigitalControlMapping(EControl::INTAKE_HOLD_UP)};
  EControllerDigital spin{
      profile->getDigitalControlMapping(EControl::INTAKE_SPIN)};
  EControllerDigital reverse{
      profile->getDigitalControlMapping(EControl::INTAKE_REVERSE)};

  if (!m_controller) {
    return;
  }

  switch (static_cast<EIntakeControlMode>(
      profile->getControlMode(EControlType::INTAKE))) {
    case EIntakeControlMode::SPLIT_TOGGLE:
      updateSplitToggle(toggle_up, toggle_down);
      break;
    case EIntakeControlMode::SINGLE_TOGGLE:
      updateSingleToggle(toggle_states);
      break;
    case EIntakeControlMode::HOLD_UP:
      updateHoldUp(hold_up);
      break;
  }

  updateSpinner(spin, reverse);
}
}  // namespace intake
}  // namespace op_control
}  // namespace driftless