#include "driftless/processes/auto_ring_rejection/AutoRingRejectionProcess.hpp"

namespace driftless {
namespace processes {
namespace auto_ring_rejection {
AutoRingRejectionProcess::AutoRingRejectionProcess(
    std::unique_ptr<IAutoRingRejector>& ring_rejector)
    : m_ring_rejector{std::move(ring_rejector)} {}

void AutoRingRejectionProcess::init() { m_ring_rejector->init(); }

void AutoRingRejectionProcess::run() { m_ring_rejector->run(); }

void AutoRingRejectionProcess::pause() { m_ring_rejector->pause(); }

void AutoRingRejectionProcess::resume() { m_ring_rejector->resume(); }

void AutoRingRejectionProcess::command(std::string command_name,
                                       va_list& args) {
  if (command_name == REJECT_RINGS_COMMAND_NAME) {
    void* temp_robot{va_arg(args, void*)};
    std::shared_ptr<driftless::robot::Robot> robot{
        *static_cast<std::shared_ptr<driftless::robot::Robot>*>(temp_robot)};
    void* temp_alliance{va_arg(args, void*)};
    std::shared_ptr<driftless::alliance::IAlliance> alliance{
        *static_cast<std::shared_ptr<driftless::alliance::IAlliance>*>(
            temp_alliance)};

    m_ring_rejector->rejectRings(robot, alliance);
  }
}

void* AutoRingRejectionProcess::state(std::string state_name) {}
}  // namespace auto_ring_rejection
}  // namespace processes
}  // namespace driftless