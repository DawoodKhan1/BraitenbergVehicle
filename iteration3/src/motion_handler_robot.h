/**
 * @file motion_handler_robot.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

#ifndef SRC_MOTION_HANDLER_ROBOT_H_
#define SRC_MOTION_HANDLER_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cassert>
#include <iostream>

#include "src/common.h"
#include "src/motion_handler.h"
#include "src/sensor_touch.h"
#include "src/communication.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Classes
 ******************************************************************************/

/**
 * @brief Class managing a Robot's speed 
 *
 * The maximum speed and min speed are restricted.
 * Also allows velocity to be set beyond these restrictions.
 */
class MotionHandlerRobot : public MotionHandler {
 public:
  explicit MotionHandlerRobot(ArenaMobileEntity * ent)
      : MotionHandler(ent) {}

  MotionHandlerRobot(const MotionHandlerRobot& other) = default;
  MotionHandlerRobot& operator=(const MotionHandlerRobot& other) = default;

  /**
  * @brief Update the speed and the pose angle according to the sensor readings.
  *
  * Currently does not change speed.
  *
  * @param[in] st A SensorTouch to be read.
  */
  void UpdateVelocity(WheelVelocity velocity);

  /**
  * @brief Updates the speed according to velocity without clamping
  *
  * @param[in] st A SensorTouch to be read.
  */
  void UpdateVelocityOverride(WheelVelocity velocity);

  // empty function to avoid compiler errors with virtual function
  void UpdateVelocity() override {}

  /**
   * @brief Increase the overall speed of the entity by speed_delta.
   */
  void IncreaseSpeed() override;

  /**
   * @brief Decrease the overall speed of the entity by speed_delta.
   */
  void DecreaseSpeed() override;

  /**
   * @brief Turn the entity to the right by angle_delta (in degrees?)
   */
  void TurnRight() override;

  /**
   * @brief Turn the entity to the left by angle_delta (in degrees?)
   */
  void TurnLeft() override;

  /**
   * @brief Scale the velocities down if they are greater than max velocity
   * or make sure that minimum velocity is always 0.
   * 
   * @param v WheelVelocity object to have its velocities clamped
   */
  WheelVelocity clampVelocities(WheelVelocity v);

 private:
  double clamp_vel(double vel);
};

NAMESPACE_END(csci3081);

#endif  // SRC_MOTION_HANDLER_ROBOT_H_
