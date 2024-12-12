#ifndef __E_CONTROL_HPP__
#define __E_CONTROL_HPP__

namespace driftless {
namespace op_control {
enum EControl {
  ARM_NEUTRAL,
  ARM_LOAD,
  ARM_READY,
  ARM_SCORE,
  ARM_RUSH,
  ARM_TOGGLE,
  ARM_CALIBRATE,
  ARM_ALLIANCE_STAKE,
  CLAMP_GRAB,
  CLAMP_RELEASE,
  CLAMP_HOLD,
  CLAMP_TOGGLE,
  CLIMB_UP,
  CLIMB_HOLD,
  CLIMB_PULL,
  CLIMB_RELEASE,
  DRIVE_ARCADE_LINEAR,
  DRIVE_ARCADE_TURN,
  ELEVATOR_SPIN,
  ELEVATOR_REVERSE,
  ELEVATOR_TOGGLE,
  ELEVATOR_TOGGLE_COLOR_SORT,
  INTAKE_SPIN,
  INTAKE_REVERSE,
  INTAKE_RAISE,
  INTAKE_LOWER,
  INTAKE_TOGGLE_HEIGHT,
  INTAKE_HOLD_UP
};
}  // namespace op_control
}  // namespace driftless
#endif