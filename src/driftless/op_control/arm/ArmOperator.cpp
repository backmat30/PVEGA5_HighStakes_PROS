#include "driftless/op_control/arm/ArmOperator.hpp"

namespace driftless {
namespace op_control {
namespace arm {
bool ArmOperator::hasAllianceRing(
    const driftless::alliance::Alliance alliance) {
  bool has_ring{*static_cast<bool*>(
      m_robot->getState(ARM_SUBSYSTEM_NAME, HAS_RING_STATE_NAME))};

  double ring_hue{*static_cast<double*>(
      m_robot->getState(ARM_SUBSYSTEM_NAME, GET_HUE_STATE_NAME))};

  bool has_alliance_ring{};
  if (ring_hue >= alliance.hue_range[0] && ring_hue <= alliance.hue_range[1]) {
    has_alliance_ring = has_ring;
  }

  return has_alliance_ring;
}
void ArmOperator::updateSplitToggle(
    EControllerDigital neutral, EControllerDigital load,
    EControllerDigital ready, EControllerDigital score,
    const driftless::alliance::Alliance alliance) {
  bool go_neutral{m_controller->getNewDigital(neutral)};
  bool go_load{m_controller->getNewDigital(load)};
  bool go_ready{m_controller->getNewDigital(ready)};
  bool go_score{m_controller->getNewDigital(score)};

  if (go_neutral && !go_load && !go_ready && !go_score) {
    bool at_load{*static_cast<bool*>(
        m_robot->getState(ARM_SUBSYSTEM_NAME, IS_LOAD_STATE_NAME))};
    if (!(hasAllianceRing(alliance) && at_load)) {
      m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_NEUTRAL_COMMAND_NAME);
    }

  } else if (!go_neutral && go_load && !go_ready && !go_score) {
    m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_LOAD_COMMAND_NAME);

  } else if (!go_neutral && !go_load && go_ready && !go_score) {
    m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_READY_COMMAND_NAME);
  } else if (!go_neutral && !go_load && go_score) {
    bool is_ready{*static_cast<bool*>(
        m_robot->getState(ARM_SUBSYSTEM_NAME, IS_READY_STATE_NAME))};

    if (is_ready) {
      m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_SCORE_COMMAND_NAME);
    }
  }
}

void ArmOperator::updateSmartToggle(
    EControllerDigital toggle, EControllerDigital rush,
    const driftless::alliance::Alliance alliance) {
  bool next_position{m_controller->getNewDigital(toggle)};
  bool go_rush{m_controller->getNewDigital(rush)};

  void* is_neutral_state{
      m_robot->getState(ARM_SUBSYSTEM_NAME, IS_NEUTRAL_STATE_NAME)};
  bool is_neutral{is_neutral_state != nullptr &&
                  *static_cast<bool*>(is_neutral_state)};

  void* is_going_neutral_state{
      m_robot->getState(ARM_SUBSYSTEM_NAME, IS_GOING_NEUTRAL_STATE_NAME)};
  bool is_going_neutral{is_going_neutral_state != nullptr &&
                        *static_cast<bool*>(is_going_neutral_state)};

  void* is_load_state{
      m_robot->getState(ARM_SUBSYSTEM_NAME, IS_LOAD_STATE_NAME)};
  bool is_load{is_load_state != nullptr && *static_cast<bool*>(is_load_state)};

  void* is_going_load_state{
      m_robot->getState(ARM_SUBSYSTEM_NAME, IS_GOING_LOAD_STATE_NAME)};
  bool is_going_load{is_going_load_state != nullptr &&
                     *static_cast<bool*>(is_going_load_state)};

  void* is_ready_state{
      m_robot->getState(ARM_SUBSYSTEM_NAME, IS_READY_STATE_NAME)};
  bool is_ready{is_ready_state != nullptr &&
                *static_cast<bool*>(is_ready_state)};

  void* is_score_state{
      m_robot->getState(ARM_SUBSYSTEM_NAME, IS_SCORE_STATE_NAME)};
  bool is_score{is_score_state != nullptr &&
                *static_cast<bool*>(is_score_state)};

  void* is_rush_state{
      m_robot->getState(ARM_SUBSYSTEM_NAME, IS_RUSH_STATE_NAME)};
  bool is_rush{is_rush_state != nullptr && *static_cast<bool*>(is_rush_state)};

  if (next_position && !go_rush) {
    if (is_neutral || is_going_neutral) {
      m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_LOAD_COMMAND_NAME);
    } else if (is_load) {
      // if (hasAllianceRing(alliance)) {
      m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_READY_COMMAND_NAME);
      // } else {
      //   m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_NEUTRAL_COMMAND_NAME);
      // }
    } else if (is_ready) {
      m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_SCORE_COMMAND_NAME);
    }
  } else if (!next_position && go_rush) {
    if (is_rush) {
      m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_NEUTRAL_COMMAND_NAME);
    } else {
      m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_RUSH_COMMAND_NAME);
    }
  }
  if (is_score) {
    m_robot->sendCommand(ARM_SUBSYSTEM_NAME, GO_LOAD_COMMAND_NAME);
  }
}

ArmOperator::ArmOperator(
    const std::shared_ptr<driftless::io::IController>& controller,
    const std::shared_ptr<driftless::robot::Robot>& robot)
    : m_controller{controller}, m_robot{robot} {}

void ArmOperator::update(
    const std::unique_ptr<driftless::profiles::IProfile>& profile,
    const driftless::alliance::Alliance alliance) {
  EControllerDigital neutral{
      profile->getDigitalControlMapping(EControl::ARM_NEUTRAL)};
  EControllerDigital load{
      profile->getDigitalControlMapping(EControl::ARM_LOAD)};
  EControllerDigital ready{
      profile->getDigitalControlMapping(EControl::ARM_READY)};
  EControllerDigital score{
      profile->getDigitalControlMapping(EControl::ARM_SCORE)};
  EControllerDigital toggle{
      profile->getDigitalControlMapping(EControl::ARM_TOGGLE)};
  EControllerDigital rush{
      profile->getDigitalControlMapping(EControl::ARM_RUSH)};

  switch (static_cast<EArmControlMode>(
      profile->getControlMode(EControlType::ARM))) {
    case EArmControlMode::SPLIT_TOGGLE:
      updateSplitToggle(neutral, load, ready, score, alliance);
      break;
    case EArmControlMode::SINGLE_TOGGLE:
      updateSmartToggle(toggle, rush, alliance);
      break;
  }
}
}  // namespace arm
}  // namespace op_control
}  // namespace driftless