#include "pvegas/control/path/PIDPathFollowerBuilder.hpp"

namespace pvegas {
namespace control {
namespace path {
PIDPathFollowerBuilder* PIDPathFollowerBuilder::withDelayer(
    std::unique_ptr<pvegas::rtos::IDelayer>& delayer) {
  m_delayer = delayer->clone();
  return this;
}

PIDPathFollowerBuilder* PIDPathFollowerBuilder::withMutex(
    std::unique_ptr<pvegas::rtos::IMutex>& mutex) {
  m_mutex = std::move(mutex);
  return this;
}

PIDPathFollowerBuilder* PIDPathFollowerBuilder::withTask(
    std::unique_ptr<pvegas::rtos::ITask>& task) {
  m_task = std::move(task);
  return this;
}

PIDPathFollowerBuilder* PIDPathFollowerBuilder::withLinearPID(
    pvegas::control::PID linear_pid) {
  m_linear_pid = linear_pid;
  return this;
}

PIDPathFollowerBuilder* PIDPathFollowerBuilder::withRotationalPID(
    pvegas::control::PID rotational_pid) {
  m_rotational_pid = rotational_pid;
  return this;
}

PIDPathFollowerBuilder* PIDPathFollowerBuilder::withFollowDistance(
    double follow_distance) {
  m_follow_distance = follow_distance;
  return this;
}

PIDPathFollowerBuilder* PIDPathFollowerBuilder::withTargetTolerance(
    double target_tolerance) {
  m_target_tolerance = target_tolerance;
  return this;
}

PIDPathFollowerBuilder* PIDPathFollowerBuilder::withTargetVelocity(
    double target_velocity) {
  m_target_velocity = target_velocity;
  return this;
}

std::unique_ptr<PIDPathFollower> PIDPathFollowerBuilder::build() {
  std::unique_ptr<PIDPathFollower> path_follower{};
  path_follower->setDelayer(m_delayer);
  path_follower->setMutex(m_mutex);
  path_follower->setTask(m_task);
  path_follower->setLinearPID(m_linear_pid);
  path_follower->setRotationalPID(m_rotational_pid);
  path_follower->setFollowDistance(m_follow_distance);
  path_follower->setTargetTolerance(m_target_tolerance);
  path_follower->setTargetVelocity(m_target_velocity);

  return path_follower;
}
}  // namespace path
}  // namespace control
}  // namespace pvegas