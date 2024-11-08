#include "pvegas/config/DefaultConfig.hpp"

namespace pvegas {
namespace config {
std::string DefaultConfig::getName() { return CONFIG_NAME; }

std::shared_ptr<control::ControlSystem> DefaultConfig::buildControlSystem() {
  // creates a new ControlSystem object through the copy constructor
  std::shared_ptr<control::ControlSystem> control_system{
      std::make_shared<control::ControlSystem>()};
  // delayer and clock passed to all controls
  std::unique_ptr<pvegas::rtos::IClock> clock{
      std::make_unique<pvegas::pros_adapters::ProsClock>()};
  std::unique_ptr<pvegas::rtos::IDelayer> delayer{
      std::make_unique<pvegas::pros_adapters::ProsDelayer>()};

  // MOTION CONTROL
  pvegas::control::motion::PIDDriveStraightBuilder pid_drive_straight_builder{};
  // objects needed for drive straight algorithm
  std::unique_ptr<pvegas::rtos::IMutex> pid_drive_straight_mutex{
      std::make_unique<pvegas::pros_adapters::ProsMutex>()};
  std::unique_ptr<pvegas::rtos::ITask> pid_drive_straight_task{
      std::make_unique<pvegas::pros_adapters::ProsTask>()};
  pvegas::control::PID pid_drive_straight_linear_pid{
      clock, PID_DRIVE_STRAIGHT_LINEAR_KP, PID_DRIVE_STRAIGHT_LINEAR_KI,
      PID_DRIVE_STRAIGHT_LINEAR_KD};
  pvegas::control::PID pid_drive_straight_rotational_pid{
      clock, PID_DRIVE_STRAIGHT_ROTATIONAL_KP, PID_DRIVE_STRAIGHT_ROTATIONAL_KI,
      PID_DRIVE_STRAIGHT_ROTATIONAL_KD};

  // assemble the drive straight object
  std::unique_ptr<pvegas::control::motion::IDriveStraight> drive_straight{
      pid_drive_straight_builder.withDelayer(delayer)
          ->withMutex(pid_drive_straight_mutex)
          ->withTask(pid_drive_straight_task)
          ->withLinearPID(pid_drive_straight_linear_pid)
          ->withRotationalPID(pid_drive_straight_rotational_pid)
          ->withTargetTolerance(PID_DRIVE_STRAIGHT_TARGET_TOLERANCE)
          ->withTargetVelocity(PID_DRIVE_STRAIGHT_TARGET_VELOCITY)
          ->build()};

  pvegas::control::motion::PIDGoToPointBuilder pid_go_to_point_builder{};
  // objects needed for go to point algorithm
  std::unique_ptr<pvegas::rtos::IMutex> pid_go_to_point_mutex{
      std::make_unique<pvegas::pros_adapters::ProsMutex>()};
  std::unique_ptr<pvegas::rtos::ITask> pid_go_to_point_task{
      std::make_unique<pvegas::pros_adapters::ProsTask>()};
  pvegas::control::PID pid_go_to_point_linear_pid{
      clock, PID_GO_TO_POINT_LINEAR_KP, PID_GO_TO_POINT_LINEAR_KI,
      PID_GO_TO_POINT_LINEAR_KD};
  pvegas::control::PID pid_go_to_point_rotational_pid{
      clock, PID_GO_TO_POINT_ROTATIONAL_KP, PID_GO_TO_POINT_ROTATIONAL_KI,
      PID_GO_TO_POINT_ROTATIONAL_KD};

  // assemble the go to point object
  std::unique_ptr<pvegas::control::motion::IGoToPoint> go_to_point{
      pid_go_to_point_builder.withDelayer(delayer)
          ->withMutex(pid_go_to_point_mutex)
          ->withTask(pid_go_to_point_task)
          ->withLinearPID(pid_go_to_point_linear_pid)
          ->withRotationalPID(pid_go_to_point_rotational_pid)
          ->withTargetTolerance(PID_GO_TO_POINT_TARGET_TOLERANCE)
          ->withTargetVelocity(PID_GO_TO_POINT_TARGET_VELOCITY)
          ->build()};

  pvegas::control::motion::PIDTurnBuilder pid_turn_builder{};
  // objects needed for turn algorithm
  std::unique_ptr<pvegas::rtos::IMutex> pid_turn_mutex{
      std::make_unique<pvegas::pros_adapters::ProsMutex>()};
  std::unique_ptr<pvegas::rtos::ITask> pid_turn_task{
      std::make_unique<pvegas::pros_adapters::ProsTask>()};
  pvegas::control::PID pid_turn_rotational_pid{clock, PID_TURN_ROTATIONAL_KP,
                                               PID_TURN_ROTATIONAL_KI,
                                               PID_TURN_ROTATIONAL_KD};

  // assemble the turn object
  std::unique_ptr<pvegas::control::motion::ITurn> turn{
      pid_turn_builder.withDelayer(delayer)
          ->withMutex(pid_turn_mutex)
          ->withTask(pid_turn_task)
          ->withRotationalPID(pid_turn_rotational_pid)
          ->withTargetTolerance(PID_TURN_TARGET_TOLERANCE)
          ->withTargetVelocity(PID_TURN_TARGET_VELOCITY)
          ->build()};

  // create and add the motion control
  std::unique_ptr<pvegas::control::AControl> motion_control{
      std::make_unique<pvegas::control::motion::MotionControl>(
          drive_straight, go_to_point, turn)};
  control_system->addControl(motion_control);

  // PATH FOLLOWER CONTROL
  pvegas::control::path::PIDPathFollowerBuilder pid_path_follower_builder{};
  // needed objects for the path follower
  std::unique_ptr<pvegas::rtos::IMutex> pid_path_follower_mutex{
      std::make_unique<pvegas::pros_adapters::ProsMutex>()};
  std::unique_ptr<pvegas::rtos::ITask> pid_path_follower_task{
      std::make_unique<pvegas::pros_adapters::ProsTask>()};
  pvegas::control::PID pid_path_follower_linear_pid{
      clock, PID_PATH_FOLLOWER_LINEAR_KP, PID_PATH_FOLLOWER_LINEAR_KI,
      PID_PATH_FOLLOWER_LINEAR_KD};
  pvegas::control::PID pid_path_follower_rotational_pid{
      clock, PID_PATH_FOLLOWER_ROTATIONAL_KP, PID_PATH_FOLLOWER_ROTATIONAL_KI,
      PID_PATH_FOLLOWER_ROTATIONAL_KD};

  // assemble the path follower
  std::unique_ptr<pvegas::control::path::IPathFollower> pid_path_follower{
      pid_path_follower_builder.withDelayer(delayer)
          ->withMutex(pid_path_follower_mutex)
          ->withTask(pid_path_follower_task)
          ->withLinearPID(pid_path_follower_linear_pid)
          ->withRotationalPID(pid_path_follower_rotational_pid)
          ->withFollowDistance(PID_PATH_FOLLOWER_FOLLOW_DISTANCE)
          ->withTargetTolerance(PID_PATH_FOLLOWER_TARGET_TOLERANCE)
          ->withTargetVelocity(PID_PATH_FOLLOWER_TARGET_VELOCITY)
          ->build()};

  // put the path follower in a control object
  std::unique_ptr<pvegas::control::AControl> path_follower_control{
      std::make_unique<pvegas::control::path::PathFollowerControl>(
          pid_path_follower)};
  // insert the path follower control into the control manager
  control_system->addControl(path_follower_control);

  return control_system;
}

std::shared_ptr<io::IController> DefaultConfig::buildController() {
  // creates a default pros controller
  std::unique_ptr<pros::Controller> pros_controller{
      std::make_unique<pros::Controller>(pros::E_CONTROLLER_MASTER)};
  // adapts the pros controller to work as an IController object
  std::shared_ptr<io::IController> adapted_controller{
      std::make_shared<pros_adapters::ProsController>(pros_controller)};
  // send back a new adapted controller
  return adapted_controller;
}

std::shared_ptr<robot::Robot> DefaultConfig::buildRobot() {
  // create the robot
  std::shared_ptr<robot::Robot> robot{std::make_shared<robot::Robot>()};

  // DRIVETRAIN

  // creates the factory used to build the drivetrain
  robot::subsystems::drivetrain::DirectDriveBuilder drive_factory{};
  // pros objects
  // left motors
  std::unique_ptr<pros::Motor> left_temp_motor_1{
      std::make_unique<pros::Motor>(DRIVE_LEFT_MOTOR_1, DRIVE_GEARSET)};
  std::unique_ptr<pros::Motor> left_temp_motor_2{
      std::make_unique<pros::Motor>(DRIVE_LEFT_MOTOR_2, DRIVE_GEARSET)};
  std::unique_ptr<pros::Motor> left_temp_motor_3{
      std::make_unique<pros::Motor>(DRIVE_LEFT_MOTOR_3, DRIVE_GEARSET)};
  std::unique_ptr<pros::Motor> left_temp_motor_4{
      std::make_unique<pros::Motor>(DRIVE_LEFT_MOTOR_4, DRIVE_GEARSET)};
  // right motors
  std::unique_ptr<pros::Motor> right_temp_motor_1{
      std::make_unique<pros::Motor>(DRIVE_RIGHT_MOTOR_1, DRIVE_GEARSET)};
  std::unique_ptr<pros::Motor> right_temp_motor_2{
      std::make_unique<pros::Motor>(DRIVE_RIGHT_MOTOR_2, DRIVE_GEARSET)};
  std::unique_ptr<pros::Motor> right_temp_motor_3{
      std::make_unique<pros::Motor>(DRIVE_RIGHT_MOTOR_3, DRIVE_GEARSET)};
  std::unique_ptr<pros::Motor> right_temp_motor_4{
      std::make_unique<pros::Motor>(DRIVE_RIGHT_MOTOR_4, DRIVE_GEARSET)};

  // pros adapters
  // left motors
  std::unique_ptr<io::IMotor> left_motor_1{
      std::make_unique<pros_adapters::ProsV5Motor>(left_temp_motor_1)};
  std::unique_ptr<io::IMotor> left_motor_2{
      std::make_unique<pros_adapters::ProsV5Motor>(left_temp_motor_2)};
  std::unique_ptr<io::IMotor> left_motor_3{
      std::make_unique<pros_adapters::ProsV5Motor>(left_temp_motor_3)};
  std::unique_ptr<io::IMotor> left_motor_4{
      std::make_unique<pros_adapters::ProsV5Motor>(left_temp_motor_4)};
  // right motors
  std::unique_ptr<io::IMotor> right_motor_1{
      std::make_unique<pros_adapters::ProsV5Motor>(right_temp_motor_1)};
  std::unique_ptr<io::IMotor> right_motor_2{
      std::make_unique<pros_adapters::ProsV5Motor>(right_temp_motor_2)};
  std::unique_ptr<io::IMotor> right_motor_3{
      std::make_unique<pros_adapters::ProsV5Motor>(right_temp_motor_3)};
  std::unique_ptr<io::IMotor> right_motor_4{
      std::make_unique<pros_adapters::ProsV5Motor>(right_temp_motor_4)};

  // assembling the drive train
  std::unique_ptr<robot::subsystems::drivetrain::IDrivetrain> drivetrain{
      // call the factory and add all necessary items for the drivetrain
      drive_factory.withLeftMotor(left_motor_1)
          ->withLeftMotor(left_motor_2)
          ->withLeftMotor(left_motor_3)
          ->withLeftMotor(left_motor_4)
          ->withRightMotor(right_motor_1)
          ->withRightMotor(right_motor_2)
          ->withRightMotor(right_motor_3)
          ->withRightMotor(right_motor_4)
          ->withVelocityToVoltage(DRIVE_VELOCITY_TO_VOLTAGE)
          ->withDriveRadius(ROBOT_RADIUS)
          ->withWheelRadius(DRIVE_WHEEL_RADIUS)
          ->build()};
  // create the subsystem for the drivetrain
  std::unique_ptr<robot::subsystems::ASubsystem> drivetrain_subsystem{
      std::make_unique<robot::subsystems::drivetrain::DrivetrainSubsystem>(
          drivetrain)};
  // add the new subsystem to the robot
  robot->addSubsystem(drivetrain_subsystem);

  // ARM

  // rtos
  std::unique_ptr<pvegas::rtos::IClock> arm_clock{
      std::make_unique<pvegas::pros_adapters::ProsClock>()};
  std::unique_ptr<pvegas::rtos::IDelayer> arm_delayer{
      std::make_unique<pvegas::pros_adapters::ProsDelayer>()};
  std::unique_ptr<pvegas::rtos::IMutex> arm_mutex{
      std::make_unique<pvegas::pros_adapters::ProsMutex>()};
  std::unique_ptr<pvegas::rtos::ITask> arm_task{
      std::make_unique<pvegas::pros_adapters::ProsTask>()};

  // pros objects
  std::unique_ptr<pros::Motor> temp_arm_left_rotation_motor{
      std::make_unique<pros::Motor>(ARM_LEFT_ROTATION_MOTOR)};
  std::unique_ptr<pros::Motor> temp_arm_right_rotation_motor{
      std::make_unique<pros::Motor>(ARM_RIGHT_ROTATION_MOTOR)};
  std::unique_ptr<pros::Motor> temp_arm_linear_motor{
      std::make_unique<pros::Motor>(ARM_LINEAR_MOTOR)};
  std::unique_ptr<pros::Optical> temp_arm_color_sensor{
      std::make_unique<pros::Optical>(ARM_COLOR_SENSOR)};
  std::unique_ptr<pros::adi::AnalogIn> temp_arm_potentiometer{
      std::make_unique<pros::adi::AnalogIn>(ARM_POTENTIOMETER)};

  // adapted objects
  std::unique_ptr<pvegas::io::IMotor> arm_left_rotation_motor{
      std::make_unique<pvegas::pros_adapters::ProsV5Motor>(
          temp_arm_left_rotation_motor)};
  std::unique_ptr<pvegas::io::IMotor> arm_right_rotation_motor{
      std::make_unique<pvegas::pros_adapters::ProsV5Motor>(
          temp_arm_right_rotation_motor)};
  std::unique_ptr<pvegas::io::IMotor> arm_linear_motor{
      std::make_unique<pvegas::pros_adapters::ProsV5Motor>(
          temp_arm_linear_motor)};
  std::unique_ptr<pvegas::io::IColorSensor> arm_color_sensor{
      std::make_unique<pvegas::pros_adapters::ProsColorSensor>(
          temp_arm_color_sensor)};
  std::unique_ptr<pvegas::io::IPotentiometer> arm_potentiometer{
      std::make_unique<pvegas::pros_adapters::ProsADIPotentiometer>(
          temp_arm_potentiometer)};

  pvegas::control::PID arm_rotational_pid{arm_clock, PID_ARM_ROTATIONAL_KP,
                                          PID_ARM_ROTATIONAL_KI,
                                          PID_ARM_ROTATIONAL_KD};
  pvegas::control::PID arm_linear_pid{arm_clock, PID_ARM_LINEAR_KP,
                                      PID_ARM_LINEAR_KI, PID_ARM_LINEAR_KD};

  // assemble the subsystem
  pvegas::robot::subsystems::arm::PIDArmMotionBuilder pid_arm_motion_builder{};
  pvegas::robot::subsystems::arm::ColorRingSensorBuilder
      color_ring_sensor_builder{};

  std::unique_ptr<pvegas::robot::subsystems::arm::IArmMotion> arm_motion{
      pid_arm_motion_builder.withDelayer(arm_delayer)
          ->withMutex(arm_mutex)
          ->withTask(arm_task)
          ->withRotationalMotor(arm_left_rotation_motor)
          ->withRotationalMotor(arm_right_rotation_motor)
          ->withLinearMotor(arm_linear_motor)
          ->withPotentiometer(arm_potentiometer)
          ->withRotationalPID(arm_rotational_pid)
          ->withLinearPID(arm_linear_pid)
          ->withRotationalNeutralPosition(ARM_ROTATIONAL_NEUTRAL_POSITION)
          ->withRotationalLoadPosition(ARM_ROTATIONAL_LOAD_POSITION)
          ->withRotationalReadyPosition(ARM_ROTATIONAL_READY_POSITION)
          ->withRotationalScorePosition(ARM_ROTATIONAL_SCORE_POSITION)
          ->withRotationalTolerance(ARM_ROTATIONAL_TOLERANCE)
          ->withLinearNeutralPosition(ARM_LINEAR_NEUTRAL_POSITION)
          ->withLinearLoadPosition(ARM_LINEAR_LOAD_POSITION)
          ->withLinearReadyPosition(ARM_LINEAR_READY_POSITION)
          ->withLinearScorePosition(ARM_LINEAR_SCORE_POSITION)
          ->withLinearTolerance(ARM_LINEAR_TOLERANCE)
          ->build()};

  std::unique_ptr<pvegas::robot::subsystems::arm::IRingSensor> ring_sensor{
      color_ring_sensor_builder.withColorSensor(arm_color_sensor)
          ->withRingProximity(ARM_RING_PROXIMITY)
          ->build()};

  std::unique_ptr<pvegas::robot::subsystems::ASubsystem> arm_subsystem{
      std::make_unique<pvegas::robot::subsystems::arm::ArmSubsystem>(
          arm_motion, ring_sensor)};

  robot->addSubsystem(arm_subsystem);

  // CLAMP

  // pros objects
  std::unique_ptr<pros::adi::DigitalOut> temp_clamp_left_piston{
      std::make_unique<pros::adi::DigitalOut>(CLAMP_PISTON_1)};

  // adapted objects
  std::unique_ptr<pvegas::io::IPiston> adapted_clamp_left_piston{
      std::make_unique<pvegas::pros_adapters::ProsPiston>(
          temp_clamp_left_piston)};

  // build the clamp
  pvegas::robot::subsystems::clamp::PistonClampBuilder piston_clamp_builder{};

  std::unique_ptr<pvegas::robot::subsystems::clamp::IClamp> piston_clamp{
      piston_clamp_builder.withPiston(adapted_clamp_left_piston)->build()};

  std::unique_ptr<pvegas::robot::subsystems::ASubsystem> clamp_subsystem{
      std::make_unique<pvegas::robot::subsystems::clamp::ClampSubsystem>(
          piston_clamp)};
  robot->addSubsystem(clamp_subsystem);

  // ELEVATOR

  // rtos
  std::unique_ptr<pvegas::rtos::IClock> elevator_clock{
      std::make_unique<pvegas::pros_adapters::ProsClock>()};
  std::unique_ptr<pvegas::rtos::IDelayer> elevator_delayer{
      std::make_unique<pvegas::pros_adapters::ProsDelayer>()};
  std::unique_ptr<pvegas::rtos::IMutex> elevator_mutex{
      std::make_unique<pvegas::pros_adapters::ProsMutex>()};
  std::unique_ptr<pvegas::rtos::ITask> elevator_task{
      std::make_unique<pvegas::pros_adapters::ProsTask>()};
  // pros objects
  std::unique_ptr<pros::Motor> temp_elevator_motor_1{
      std::make_unique<pros::Motor>(ELEVATOR_MOTOR_1)};
  std::unique_ptr<pros::Rotation> temp_elevator_rotation_sensor{
      std::make_unique<pros::Rotation>(ELEVATOR_ROTATIONAL_SENSOR)};

  // adapted objects
  std::unique_ptr<pvegas::io::IMotor> adapted_elevator_motor_1{
      std::make_unique<pvegas::pros_adapters::ProsV5Motor>(
          temp_elevator_motor_1)};
  std::unique_ptr<pvegas::io::IRotationSensor>
      adapted_elevator_rotational_sensor{
          std::make_unique<pvegas::pros_adapters::ProsRotationSensor>(
              temp_elevator_rotation_sensor)};

  pvegas::control::PID elevator_pid{elevator_clock, PID_ELEVATOR_KP,
                                    PID_ELEVATOR_KI, PID_ELEVATOR_KD};

  // build the elevator
  pvegas::robot::subsystems::elevator::PIDElevatorBuilder
      pid_elevator_builder{};

  std::unique_ptr<pvegas::robot::subsystems::elevator::IElevator> elevator{
      pid_elevator_builder.withDelayer(elevator_delayer)
          ->withMutex(elevator_mutex)
          ->withTask(elevator_task)
          ->withMotor(adapted_elevator_motor_1)
          ->withRotationSensor(adapted_elevator_rotational_sensor)
          ->withPID(elevator_pid)
          ->withRadiansToInches(ELEVATOR_RADIANS_TO_INCHES)
          ->build()};

  std::unique_ptr<pvegas::robot::subsystems::ASubsystem> elevator_subsystem{
      std::make_unique<pvegas::robot::subsystems::elevator::ElevatorSubsystem>(
          elevator)};
  robot->addSubsystem(elevator_subsystem);

  // INTAKE

  // pros objects
  std::unique_ptr<pros::Motor> temp_intake_motor_1{
      std::make_unique<pros::Motor>(INTAKE_MOTOR)};
  std::unique_ptr<pros::adi::DigitalOut> temp_intake_left_piston{
      std::make_unique<pros::adi::DigitalOut>(INTAKE_LEFT_PISTON)};
  std::unique_ptr<pros::adi::DigitalOut> temp_intake_right_piston{
      std::make_unique<pros::adi::DigitalOut>(INTAKE_RIGHT_PISTON)};

  // adapted objects
  std::unique_ptr<pvegas::io::IMotor> intake_motor_1{
      std::make_unique<pvegas::pros_adapters::ProsV5Motor>(
          temp_intake_motor_1)};
  std::unique_ptr<pvegas::io::IPiston> intake_left_piston{
      std::make_unique<pvegas::pros_adapters::ProsPiston>(
          temp_intake_left_piston)};
  std::unique_ptr<pvegas::io::IPiston> intake_right_piston{
      std::make_unique<pvegas::pros_adapters::ProsPiston>(
          temp_intake_right_piston)};

  // build the intake
  pvegas::robot::subsystems::intake::DirectIntakeBuilder
      direct_intake_builder{};
  pvegas::robot::subsystems::intake::PistonHeightControlBuilder
      piston_height_control_builder{};

  std::unique_ptr<pvegas::robot::subsystems::intake::IIntake> direct_intake{
      direct_intake_builder.withMotor(intake_motor_1)->build()};
  std::unique_ptr<pvegas::robot::subsystems::intake::IHeightControl>
      piston_height_control{
          piston_height_control_builder.withPiston(intake_left_piston)
              ->withPiston(intake_right_piston)
              ->build()};

  // create and add the intake subsystem to the robot
  std::unique_ptr<pvegas::robot::subsystems::ASubsystem> intake_subsystem{
      std::make_unique<pvegas::robot::subsystems::intake::IntakeSubsystem>(
          direct_intake, piston_height_control)};
  robot->addSubsystem(intake_subsystem);

  // ODOMETRY

  // pros objects
  std::unique_ptr<pros::Rotation> temp_linear_rotation_sensor{
      std::make_unique<pros::Rotation>(ODOMETRY_LINEAR_TRACKING_WHEEL)};
  std::unique_ptr<pros::Rotation> temp_strafe_rotation_sensor{
      std::make_unique<pros::Rotation>(ODOMETRY_STRAFE_TRACKING_WHEEL)};
  std::unique_ptr<pros::IMU> temp_inertial_sensor{
      std::make_unique<pros::IMU>(ODOMETRY_INERTIAL_SENSOR)};
  std::unique_ptr<pros::Distance> temp_distance_sensor{
      std::make_unique<pros::Distance>(ODOMETRY_DISTANCE_SENSOR)};

  // adapted objects
  // linear tracking wheel
  std::unique_ptr<pvegas::io::IRotationSensor> linear_rotation_sensor{
      std::make_unique<pvegas::pros_adapters::ProsRotationSensor>(
          temp_linear_rotation_sensor)};
  std::unique_ptr<pvegas::io::IDistanceTracker> linear_tracking_wheel{
      std::make_unique<pvegas::hal::TrackingWheel>(linear_rotation_sensor,
                                                   TRACKING_WHEEL_RADIUS)};
  // strafe tracking wheel
  std::unique_ptr<pvegas::io::IRotationSensor> strafe_rotation_sensor{
      std::make_unique<pvegas::pros_adapters::ProsRotationSensor>(
          temp_strafe_rotation_sensor)};
  std::unique_ptr<pvegas::io::IDistanceTracker> strafe_tracking_wheel{
      std::make_unique<pvegas::hal::TrackingWheel>(strafe_rotation_sensor,
                                                   TRACKING_WHEEL_RADIUS)};
  // inertial sensor
  std::unique_ptr<pvegas::io::IInertialSensor> inertial_sensor{
      std::make_unique<pvegas::pros_adapters::ProsInertialSensor>(
          temp_inertial_sensor)};
  // distance sensor
  std::unique_ptr<pvegas::io::IDistanceSensor> distance_sensor{
      std::make_unique<pvegas::pros_adapters::ProsDistanceSensor>(
          temp_distance_sensor)};
  // odometry rtos
  std::unique_ptr<pvegas::rtos::IClock> odometry_clock{
      std::make_unique<pvegas::pros_adapters::ProsClock>()};
  std::unique_ptr<pvegas::rtos::IDelayer> odometry_delayer{
      std::make_unique<pvegas::pros_adapters::ProsDelayer>()};
  std::unique_ptr<pvegas::rtos::IMutex> odometry_mutex{
      std::make_unique<pvegas::pros_adapters::ProsMutex>()};
  std::unique_ptr<pvegas::rtos::ITask> odometry_task{
      std::make_unique<pvegas::pros_adapters::ProsTask>()};
  // position tracker
  pvegas::robot::subsystems::odometry::InertialPositionTrackerBuilder
      inertial_position_tracker_builder{};
  std::unique_ptr<pvegas::robot::subsystems::odometry::IPositionTracker>
      inertial_position_tracker{
          inertial_position_tracker_builder.withClock(odometry_clock)
              ->withDelayer(odometry_delayer)
              ->withMutex(odometry_mutex)
              ->withTask(odometry_task)
              ->withInertialSensor(inertial_sensor)
              ->withLinearDistanceTracker(linear_tracking_wheel)
              ->withLinearDistanceTrackerOffset(LINEAR_TRACKING_WHEEL_OFFSET)
              ->withStrafeDistanceTracker(strafe_tracking_wheel)
              ->withStrafeDistanceTrackerOffset(STRAFE_TRACKING_WHEEL_OFFSET)
              ->build()};
  // position resetter
  pvegas::robot::subsystems::odometry::DistancePositionResetterBuilder
      distance_position_resetter_builder{};
  std::unique_ptr<pvegas::robot::subsystems::odometry::IPositionResetter>
      distance_position_resetter{
          distance_position_resetter_builder
              .withDistanceSensor(distance_sensor)
              ->withLocalX(RESETTER_LOCAL_X_OFFSET)
              ->withLocalY(RESETTER_LOCAL_Y_OFFSET)
              ->withLocalTheta(RESETTER_LOCAL_THETA_OFFSET)
              ->build()};

  // create and add the subsystem
  std::unique_ptr<pvegas::robot::subsystems::ASubsystem> odometry_subsystem{
      std::make_unique<pvegas::robot::subsystems::odometry::OdometrySubsystem>(
          inertial_position_tracker, distance_position_resetter)};
  robot->addSubsystem(odometry_subsystem);

  // send out the finalized robot
  return robot;
}
}  // namespace config
}  // namespace pvegas