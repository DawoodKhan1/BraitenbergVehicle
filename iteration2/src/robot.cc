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
    left_lightsensor_(new LightSensor()),
    right_lightsensor_(new LightSensor()),
    left_foodsensor_(new FoodSensor()),
    right_foodsensor_(new FoodSensor()),
    robot_type_(kFear),
    robot_behavior_(new FearBehavior()) {
  set_type(kRobot);
  set_color(ROBOT_COLOR);
  set_pose(ROBOT_INIT_POS);
  set_radius(ROBOT_RADIUS);

  // set the absolute position of the sensor
  left_lightsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  right_lightsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  left_foodsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  right_foodsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());

  // left sensor should be left of angle and right is positive angle
  left_lightsensor_->set_angle_offset(-ANGLE_OFFSET);
  right_lightsensor_->set_angle_offset(ANGLE_OFFSET);
  left_foodsensor_->set_angle_offset(-ANGLE_OFFSET);
  right_foodsensor_->set_angle_offset(ANGLE_OFFSET);

  // coloring light sensors white
  left_lightsensor_->set_color(kWhite);
  right_lightsensor_->set_color(kWhite);

  // coloring food sensors red
  left_foodsensor_->set_color(kRed);
  right_foodsensor_->set_color(kRed);

  left_lightsensor_->set_radius(3.0);
  right_lightsensor_->set_radius(3.0);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Robot::TimestepUpdate(unsigned int dt) {
  // update the time and any flags as needed
  time_since_last_meal_ += dt;

  // update flags based on time
  if (time_since_last_meal_ >= hungry_time_)
    hungry_ = true;
  if (time_since_last_meal_ >= starving_time_)
    starving_ = true;
  if (time_since_last_meal_ >= starved_time_)
    starved_ = true;

  // Update robot's position
  left_lightsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  right_lightsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  left_foodsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  right_foodsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());

  // velocity for updating for override or sensors
  WheelVelocity velocity;

  // if control overrides in place then start override
  if (collision_override_) {
    // update the override velocities and the counter
    velocity.left = override_velocity.left;
    velocity.right = override_velocity.right;
    collision_override_counter_ += dt;

    // check if collision overrides have expired, if so reset variables
    if (collision_override_counter_ >= collision_override_duration_) {
      collision_override_ = false;
      collision_override_counter_ = 0;
      // at the end of the reversing increment the angle so the robot doesn't
      // get stuck against the wall multiple times
      this->set_heading(this->get_heading() + 90);
    }

    motion_handler_.UpdateVelocityOverride(velocity);
  } else {  // if not override controls use sensor readings
    // Make WheelVelocity based on the behavior of the robot if
    // robot is not starving

    if (!starving_) {
      velocity = robot_behavior_->processReading(
        left_lightsensor_->get_reading(), right_lightsensor_->get_reading());
    }

    // now combine light sensor readings with food sensor which will always be
    // agressive (+) crossed if the robot is hungry
    if (hungry_) {
      velocity = food_behavior_->processReading(
        left_foodsensor_->get_reading(), right_foodsensor_->get_reading());
    }
    if (starving_) {
      velocity = food_behavior_->processReading(
        left_foodsensor_->get_reading(), right_foodsensor_->get_reading());
    }

    motion_handler_.UpdateVelocity(velocity);
  }
  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());

  // Reset sensors for next cycle
  sensor_touch_->Reset();
  left_lightsensor_->set_reading(0.0);
  right_lightsensor_->set_reading(0.0);
  left_foodsensor_->set_reading(0.0);
  right_foodsensor_->set_reading(0.0);
} /* TimestepUpdate() */

void Robot::Reset() {
  set_pose(SetPoseRandomly());
  motion_handler_.set_max_speed(ROBOT_MAX_SPEED);
  motion_handler_.set_max_angle(ROBOT_MAX_ANGLE);
  sensor_touch_->Reset();

  motion_handler_.set_velocity(0, 0);
  hungry_ = false;
  starving_ = false;
  starved_ = false;
  time_since_last_meal_ = 0;

  // Update sensors position based on the robot's position
  left_lightsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  right_lightsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  left_foodsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
  right_foodsensor_->setSensorPositionBasedOnRobotPosition(this->get_pose());
} /* Reset() */

void Robot::HandleCollision(EntityType object_type, ArenaEntity * object) {
  switch (object_type) {
    // Check for colliding against wall or other robots
    case kRobot:
    case kRightWall:
    case kLeftWall:
    case kTopWall:
    case kBottomWall:
      motion_handler_.set_velocity(0, 0);
      sensor_touch_->HandleCollision(object_type, object);

      collision_override_ = true;  // start override controls
      break;
    case kFood:  // if robot eats from food source reset hunger states and time
      time_since_last_meal_ = 0;
      hungry_ = false;
      starving_ = false;
      break;
    case kLight:
      break;
    default:
      break;
  }
}

std::string Robot::get_name() const {
  std::string behavior;

  switch (robot_type_) {
    case kFear:
      behavior = "Fear";
      break;
    case kAgressive:
        behavior = "Aggressive";
      break;
    case kLove:
        behavior = "Love";
      break;
    case kExplore:
        behavior = "Explore";
      break;
    default:  // no default case but need it to compile
      break;
  }
  return behavior;
}

NAMESPACE_END(csci3081);
