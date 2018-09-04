/**
 * @file Light.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_LIGHT_H_
#define SRC_LIGHT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_mobile_entity.h"
#include "src/motion_behavior_differential.h"
#include "src/common.h"
#include "src/entity_type.h"
#include "src/pose.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing an mobile Light within the Arena.
 *
 * Arena sets the time and calls methods of this class upon timesteps and 
 * collisions.
 *
 * Lights move and upon any collisions reverse in an arc determined
 * by flags and timing of the simulation.
*/
class Light : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */
  Light();

  /**
   * @brief Get the name of the Light for visualization purposes, and to
   * aid in debugging.
   */
  std::string get_name() const override {
    return "Light" + std::to_string(get_id());
  }

  /**
   * @brief Reset the Light to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;

  /**
   * @brief Update the Lights's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;

  /**
   * @brief Handles the collision by setting the sensor to activated.
   */
  void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);

  double get_time() const {return time_; }

  void set_time(double time) { time_ = time; }

 private:
  // Manages pose and wheel velocities that change with time and collisions.
  WheelVelocity motion_handler_velocity{2, 2};
  // Calculates changes in pose Foodd on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;

  // constant Wheel Velocity for reversing
  WheelVelocity reverseArc{-6, -3.5};
  // constant Wheel Velocity
  WheelVelocity defaultSpeed{2, 2};

  // flag indicating whether the robot is currently reversing in an arc
  bool reverse_{false};
  // time which the reversing starts, no getters or setters
  double reverse_start_{0.0};
  // how long robot is reversing for following a collision
  double reverse_duration_{.200};
  // this is the current time updated by arena every timestep
  // this is used for timing of reversing
  double time_{0.0};
};

NAMESPACE_END(csci3081);

#endif  // SRC_LIGHT_H_
