/**
 * @file robot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler_robot.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/robot_type.h"
#include "src/robot_behavior.h"
#include "src/agressive_behavior.h"
#include "src/love_behavior.h"
#include "src/explore_behavior.h"
#include "src/fear_behavior.h"
#include "src/food.h"
#include "src/light_sensor.h"
#include "src/food_sensor.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class MotionBehaviorDifferential;
class Food;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a robot within the arena.
 *
 * Robots are composed of a motion handler, motion behavior, and touch sensor.
 * These classes interact to maintain the pose (position and heading) of the
 * robot. At each time step, the wheel velocities are used to calculate the
 * next pose of the robot. The handler manages the pose and user requests. The
 * behavior calculates the new pose Foodd on wheel velocities.
 *
 *
 * The touch sensor is activated when the robot collides with an object.
 * The heading is modified after a collision to move the robot away from the
 * other object.
 *
 * Robots reverse after colliding with another robot or the wall by reversing
 * in an arc and then incrementing the heading angle to avoid future collisions.
 *
 * Robots have 4 sensors, 2 light and 2 food sensors. These sensors are on
 * the left and right of the robot and always has a reading.
 *
 * Based on the robot's behavior, some of these readings are used and others
 * are ignored.
 * Ex. If robott is not hungry, food sensor readings are ignored.
 */
class Robot : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor using initialization values from params.h.
   */
  Robot();

  Robot(const Robot &other) = default;
  Robot &operator=(const Robot &other) = default;
  ~Robot() = default;

  /**
   * @brief Reset the Robot to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;

  /**
   * @brief Update the Robot's position and velocity after the specified
   * duration has passed.
   *
   * This function also checks for when the invincibility expires and when
   * it does it sets invincibility to false.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;

  /**
   * @brief Handles the collision by setting the sensor to activated.
   * Also sets invincibility to true and records the current time.
   */
  void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);

  /**
   * @brief Get the behavior of the Robot for visualization and for debugging.
   */
  std::string get_name() const override;

  /**
   * @brief Getter for motion_handler_
   */
  MotionHandlerRobot get_motion_handler() const { return motion_handler_; }

  /**
   * @brief Getter for motion_behavior_
   */
  MotionBehaviorDifferential get_motion_behavior() const {
    return motion_behavior_; }

  LightSensor * get_left_lightsensor() const { return left_lightsensor_; }

  LightSensor * get_right_lightsensor() const { return right_lightsensor_; }

  FoodSensor * get_left_foodsensor() const { return left_foodsensor_; }

  FoodSensor * get_right_foodsensor() const { return right_foodsensor_; }

  RobotType get_robot_type() const { return robot_type_; }

  void set_robot_type(RobotType robotType) { robot_type_ = robotType; }

  RobotBehavior * get_robot_behavior() const { return robot_behavior_; }

  void set_robot_behavior(RobotBehavior * robotBehavior) {
    robot_behavior_ = robotBehavior; }

  bool get_hungry() const { return hungry_; }

  void set_hungry(bool hungry) { hungry_ = hungry; }

  bool get_starving() const { return starving_; }

  void set_starving(bool starving) { starving_ = starving; }

  bool get_starved() const { return starved_; }

  void set_starved(bool starved) { starved_ = starved; }

  bool get_ignore_hunger() { return ignore_hunger_; }

  void set_ignore_hunger(bool ignoreHunger) { ignore_hunger_ = ignoreHunger; }

  bool get_time_since_last_meal() { return time_since_last_meal_; }

  void set_time_since_last_meal(int timeSinceLastMeal) {
    time_since_last_meal_ = timeSinceLastMeal; }

 private:
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandlerRobot motion_handler_;
  // Calculates changes in pose Foodd on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;

  // two light sensors for sensing light.
  LightSensor * left_lightsensor_;
  LightSensor * right_lightsensor_;

  // two food sensors for sensing food
  FoodSensor * left_foodsensor_;
  FoodSensor * right_foodsensor_;

  RobotType robot_type_;  // enum for robot type for behavior

  // pointer for calling method processReading for converting readings to
  // wheel velocity object
  RobotBehavior * robot_behavior_;
  // robot will always be agressive towards food if hungry
  RobotBehavior * food_behavior_ = new AgressiveBehavior();

  // flags for hunger states
  // hungry_: if false only senses light, if true senses light and food
  bool hungry_{false},
      starving_{false},  // only senses food
      starved_{false},  // if any robot has this to be true, simulation ends
      ignore_hunger_{false};  // whether to ignore all hunger or not
  // time it takes to reach the various states
  int hungry_time_{480},  // approximately 30s
      starving_time_{1920},  // approximately 120s
      starved_time_{2400};  // approximately 150s

  // time since robot has last collided with a Food object (internal timer)
  int time_since_last_meal_{0};

  // time which the override controls last from a collsion
  int collision_override_duration_{10},
      collision_override_counter_{0};
  bool collision_override_{false};
  // velocity robot will have when override is active
  WheelVelocity override_velocity_{-10, -9};
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_H_
