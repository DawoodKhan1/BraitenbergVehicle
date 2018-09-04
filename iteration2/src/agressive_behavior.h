/**
 * @file agressive_behavior.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_AGRESSIVE_BEHAVIOR_H_
#define SRC_AGRESSIVE_BEHAVIOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot_behavior.h"
#include "src/wheel_velocity.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing agressive behavior (+) crossed
 *
 */
class AgressiveBehavior : public RobotBehavior {
 public:
  ~AgressiveBehavior() = default;

  /**
   * @brief Processes the reading and then returns a WheelVelocity object 
   *        based on the readings and behavior.
   *
   * @param leftReading reading from left sensor
   * @param rightReading reading from right sensor
   */
  WheelVelocity processReading(double leftReading, double rightReading)
    override;
};

NAMESPACE_END(csci3081);

#endif  // SRC_AGRESSIVE_BEHAVIOR_H_
