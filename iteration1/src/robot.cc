/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include "src/robot.h"
#include "src/params.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Robot::Robot() :
    motion_handler_(this),
    motion_behavior_(this),
    lives_(9) {
  set_type(kRobot);
  set_color(ROBOT_COLOR);
  set_pose(ROBOT_INIT_POS);
  set_radius(ROBOT_RADIUS);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Robot::TimestepUpdate(unsigned int dt) {
  // Update heading as indicated by touch sensor
  motion_handler_.UpdateVelocity();

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());

  // Reset Sensor for next cycle
  sensor_touch_->Reset();

  // if timer has run out for invincibility take it away
  if (invincible_ && time_ >= invincibility_start_ + invincibility_duration_)
    invincible_ = false;
} /* TimestepUpdate() */

void Robot::Reset() {
  set_pose(SetPoseRandomly());
  motion_handler_.set_max_speed(ROBOT_MAX_SPEED);
  motion_handler_.set_max_angle(ROBOT_MAX_ANGLE);
  sensor_touch_->Reset();

  lives_ = 9;
  motion_handler_.set_velocity(0, 0);
  invincible_ = false;
} /* Reset() */

void Robot::HandleCollision(EntityType object_type, ArenaEntity * object) {
  // robot should stop moving after any collisions
  motion_handler_.set_velocity(0, 0);
  sensor_touch_->HandleCollision(object_type, object);


  switch (object_type) {
    // Check for colliding against wall or obstacle and decrement lives
    case kObstacle:
    case kRightWall:
    case kLeftWall:
    case kTopWall:
    case kBottomWall:
      // if not invincible and had a collision then set up
      // vars to be invincible for next 2 seconds
      if (!invincible_ && lives_ > 0) {  // prevent negative lives
        lives_--;
        invincible_ = true;
        invincibility_start_ = time_;
      }
      break;
    case kBase: {  // if base set base to captured with color green
      RgbColor baseCapturedColor;
      baseCapturedColor.Set(kGreen);
      object->set_color(baseCapturedColor);

      Base* basePtr = dynamic_cast<Base*>(object);
      basePtr->set_captured(true);
      break;
    }
    default:
      break;
  }
}

void Robot::IncreaseSpeed() {
  motion_handler_.IncreaseSpeed();
}
void Robot::DecreaseSpeed() {
  motion_handler_.DecreaseSpeed();
}
void Robot::TurnRight() {
  motion_handler_.TurnRight();
}
void Robot::TurnLeft() {
  motion_handler_.TurnLeft();
}

NAMESPACE_END(csci3081);
