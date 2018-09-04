/**
 * @file robot_behavior.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_BEHAVIOR_H_
#define SRC_ROBOT_BEHAVIOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "src/wheel_velocity.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a parent class for robot behavior.
 *
 */
class RobotBehavior {
 public:
  virtual ~RobotBehavior() = default;

  /**
   * @brief Processes the reading and then returns a WheelVelocity object 
   *        based on the readings and behavior.
   *
   * @param leftReading reading from left sensor
   * @param rightReading reading from right sensor
   */
  virtual WheelVelocity processReading(__unused double leftReading,
      __unused double rightReading) {
    return WheelVelocity();
  }

  void set_light_max_reading(double lightMaxReading) {
    light_max_reading_ = lightMaxReading;
  }

  double get_light_max_reading() { return light_max_reading_; }

 protected:
  // Based on 4 lights and formula in Notify, (1200 / 60 ^ 1.08) * 4 = 57.6
  // then round up. 60 is min possible distance 30 for robot and light radii
  double light_max_reading_ = 60.0;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_BEHAVIOR_H_
