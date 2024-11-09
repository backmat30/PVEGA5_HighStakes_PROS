#ifndef __E_TURN_DIRECTION_HPP__
#define __E_TURN_DIRECTION_HPP__

/// @brief Namespace for driftless library code
namespace driftless {
/// @brief Namespace for control algorithms
namespace control {
/// @brief Namespace for basic motion control algorithms
namespace motion {
/// @brief directions the robot can turn in
/// @details - __AUTO__ goes in the direction with the shortest path
/// - __CLOCKWISE__ goes in the _negative_ direction
/// - __COUNTERCLOCKWISE__ goes in the _positive_ direction
enum class ETurnDirection { AUTO, CLOCKWISE, COUNTERCLOCKWISE };
}  // namespace motion
}  // namespace control
}  // namespace driftless
#endif