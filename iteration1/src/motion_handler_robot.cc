/**
 * @file motion_handler_robot.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_robot.h"
#include "src/motion_behavior_differential.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
// @TODO add clamped

void MotionHandlerRobot::TurnLeft() {
  set_velocity(
    clamp_vel(get_velocity().left  - get_angle_delta()),
    clamp_vel(get_velocity().right + get_angle_delta()));
}

void MotionHandlerRobot::TurnRight() {
  set_velocity(
    clamp_vel(get_velocity().left  + get_angle_delta()),
    clamp_vel(get_velocity().right - get_angle_delta()));
}

void MotionHandlerRobot::IncreaseSpeed() {
  set_velocity(
    clamp_vel(get_velocity().left  + get_speed_delta()),
    clamp_vel(get_velocity().right + get_speed_delta()));
}

void MotionHandlerRobot::DecreaseSpeed() {
  set_velocity(
    clamp_vel(get_velocity().left  - get_speed_delta()),
    clamp_vel(get_velocity().right - get_speed_delta()));
}

void MotionHandlerRobot::UpdateVelocity() {
  if (entity_->get_touch_sensor()->get_output()) {
    // entity_->RelativeChangeHeading(+180);
  }
}

// note: this functions assumes vel is the new velocity, not the current one
double MotionHandlerRobot::clamp_vel(double vel) {
  double clamped = 0.0;  // default speed will be 0
  if (vel > 0) {
    // if positive, check if new speed will be greater than maximum
    // if new speed is greater than max set speed to max speed
    clamped = (vel > get_max_speed()) ?
              get_max_speed():
              vel;
  } else if (vel < 0) {
      clamped = 0;  // if new speed will be neg. than set speed to 0
    }

  return clamped;
} /* clamp_vel() */

NAMESPACE_END(csci3081);
