#include "driftless/robot/subsystems/clamp/PistonClamp.hpp"

namespace driftless {
namespace robot {
namespace subsystems {
namespace clamp {
void PistonClamp::init() {}

void PistonClamp::run() {}

void PistonClamp::setState(bool clamped) {
  if (clamped) {
    m_pistons.extend();
  } else {
    m_pistons.retract();
  }
  state = clamped;
}

bool PistonClamp::getState() { return state; }

void PistonClamp::setPistons(driftless::hal::PistonGroup& pistons) {
  m_pistons = std::move(pistons);
}
}  // namespace clamp
}  // namespace subsystems
}  // namespace robot
}  // namespace driftless