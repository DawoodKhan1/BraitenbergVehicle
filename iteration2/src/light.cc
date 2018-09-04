/**
 * @file Light.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/light.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Light::Light() : motion_behavior_(this) {
  set_color(Light_COLOR);
  set_pose(Light_POSITION);
  set_radius(Light_RADIUS);
  set_type(kLight);
}

void Light::Reset() {
  set_pose(SetPoseRandomly());
  set_radius(random() % (Light_MAX_RADIUS - Light_MIN_RADIUS + 1) +
    Light_MIN_RADIUS);
} /* Reset() */

void Light::TimestepUpdate(unsigned int dt) {
  if (time_ >= reverse_start_ + reverse_duration_) {
    reverse_ = false;
    motion_handler_velocity = defaultSpeed;
  }

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_velocity);

  // Reset Sensor for next cycle
  sensor_touch_->Reset();
} /* TimestepUpdate() */

void Light::HandleCollision(EntityType object_type, ArenaEntity * object) {
  // Lights should reverse arc after collisions with other lights and the wall
  if (object_type == kLight || object_type == kRightWall ||
      object_type == kLeftWall || object_type == kTopWall ||
      object_type == kBottomWall) {
    reverse_start_ = time_;
    reverse_ = true;
    motion_handler_velocity = reverseArc;

    sensor_touch_->HandleCollision(object_type, object);
  }
}

NAMESPACE_END(csci3081);
