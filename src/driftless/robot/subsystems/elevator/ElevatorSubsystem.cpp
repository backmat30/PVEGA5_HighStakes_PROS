#include "driftless/robot/subsystems/elevator/ElevatorSubsystem.hpp"

namespace driftless {
namespace robot {
namespace subsystems {
namespace elevator {
ElevatorSubsystem::ElevatorSubsystem(
    std::unique_ptr<IElevator>& elevator,
    std::unique_ptr<IRingRejection>& ring_rejector)
    : ASubsystem{SUBSYSTEM_NAME},
      m_elevator{std::move(elevator)},
      m_ring_rejector{std::move(ring_rejector)} {}

void ElevatorSubsystem::init() {
  m_elevator->init();
  m_ring_rejector->init();
}

void ElevatorSubsystem::run() {
  m_elevator->run();
  m_ring_rejector->run();
}

void ElevatorSubsystem::command(std::string command_name, va_list& args) {
  if (command_name == SET_POSITION_COMMAND_NAME) {
    double position{va_arg(args, double)};
    m_elevator->setPosition(position);
  } else if (command_name == SET_VOLTAGE_COMMAND_NAME) {
    double voltage{va_arg(args, double)};
    m_elevator->setVoltage(voltage);
  } else if (command_name == DEPLOY_REJECTOR_COMMAND_NAME) {
    m_ring_rejector->deploy();
  } else if (command_name == RETRACT_REJECTOR_COMMAND_NAME) {
    m_ring_rejector->retract();
  }
}

void* ElevatorSubsystem::state(std::string state_name) {
  void* result{nullptr};
  if (state_name == GET_POSITION_STATE_NAME) {
    result = new double{m_elevator->getPosition()};
  } else if (state_name == IS_DEPLOYED_STATE_NAME) {
    result = new bool{m_ring_rejector->isDeployed()};
  }

  return result;
}
}  // namespace elevator
}  // namespace subsystems
}  // namespace robot
}  // namespace driftless