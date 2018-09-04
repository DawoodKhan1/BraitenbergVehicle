/**
 * @file obstacle.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/obstacle.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Obstacle::Obstacle() : motion_behavior_(this) {
  set_color(OBSTACLE_COLOR);
  set_pose(OBSTACLE_POSITION);
  set_radius(OBSTACLE_RADIUS);
  set_type(kObstacle);
}

void Obstacle::Reset() {
  set_pose(SetPoseRandomly());
  set_radius(random() % (OBSTACLE_MAX_RADIUS - OBSTACLE_MIN_RADIUS + 1) +
    OBSTACLE_MIN_RADIUS);
} /* Reset() */

void Obstacle::TimestepUpdate(unsigned int dt) {
  if (time_ >= reverse_start_ + reverse_duration_) {
    reverse_ = false;
    motion_handler_velocity = defaultSpeed;
  }

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_velocity);

  // Reset Sensor for next cycle
  sensor_touch_->Reset();
} /* TimestepUpdate() */

void Obstacle::HandleCollision(EntityType object_type, ArenaEntity * object) {
  // obstacles should reverse arc after any collisions
  reverse_start_ = time_;
  reverse_ = true;
  motion_handler_velocity = reverseArc;

  sensor_touch_->HandleCollision(object_type, object);
}

NAMESPACE_END(csci3081);
