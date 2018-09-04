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
#include "src/base.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class MotionBehaviorDifferential;
class Base;

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
 * behavior calculates the new pose based on wheel velocities.
 *
 * Robots can be controlled through keypress, which modify wheel velocities.
 *
 * The touch sensor is activated when the robot collides with an object.
 * The heading is modified after a collision to move the robot away from the
 * other object.
 * 
 * Robot's keep track of invincibility by turning it on after a collision,
 * setting a flag, and checking when it runs out in TimestepUpdate.
 */
class Robot : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor using initialization values from params.h.
   */

  Robot();

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
   * @brief Get the name of the Robot for visualization and for debugging.
   */
  std::string get_name() const override { return "Robot"; }

  /**
   * @brief Command that comes from the controller, then is passed to handler.
   */
  void IncreaseSpeed();

  /**
  * @brief Command that comes from the controller, then is passed to handler.
  */
  void DecreaseSpeed();

  /**
  * @brief Command that comes from the controller, then is passed to handler.
  */
  void TurnRight();

  /**
  * @brief Command that comes from the controller, then is passed to handler.
  */
  void TurnLeft();

  int get_lives() const { return lives_; }

  void set_lives(int l) { lives_ = l; }

  bool get_invincible() const {return invincible_; }

  void set_invincible(bool invincible) { invincible_ = invincible; }

  double get_time() const {return time_; }

  void set_time(double time) { time_ = time; }

  /**
   * @brief Getter for motion_handler_
   */
  MotionHandlerRobot get_motion_handler() {return motion_handler_;}

  /**
   * @brief Getter for motion_behavior_
   */
  MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}

 private:
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandlerRobot motion_handler_;
  // Calculates changes in pose based on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;
  // Lives are decremented when the robot collides with anything.
  // When all the lives are gone, the game is lost.
  int lives_;

  // flag indicating whether the robot is currently invincible
  bool invincible_{false};

  // time which the invincibility starts, no getters or setters
  double invincibility_start_{0.0};

  /* how long robot is invincibile following a collision
     .555 duration is determined by manual testing meaning that
     .555 s in the simulation is ~ 2s in real time
      no getters or setters */
  double invincibility_duration_{.555};

  // this is the current time updated by arena every timestep
  // this is used for calculating invincibility
  double time_{0.0};
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_H_
