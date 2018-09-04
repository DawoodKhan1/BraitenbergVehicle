/**
 * @file arena.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_H_
#define SRC_ARENA_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cmath>
#include <iostream>
#include <vector>

#include "src/common.h"
#include "src/food.h"
#include "src/entity_factory.h"
#include "src/robot.h"
#include "src/communication.h"
#include "src/robot_type.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
struct arena_params;

/**
 * @brief The main class for the simulation of a 2D world with many entities
 * running around.
 *
 * While GraphicsArenaViewer handles the graphics, Arena handles all the
 * data and all the entities (it is the model of MVC). It manages the
 * interaction among the entities in the Arena.
 * 
 * The arena sends information about the location of various objects to the 
 * robot's sensors. 
 *
 */

class Arena {
 public:
  /**
   * @brief Arena's constructor.
   *
   * @param params A arena_params passed down from main.cc for the
   * initialization of Arena and the entities therein.
   *
   * Initialize all private variables and entities.
   */
  explicit Arena(const struct arena_params *const params);

  /**
   * @brief Arena's destructor. `delete` all entities created.
   */
  ~Arena();

  /**
   * @brief Advance the simulation by the specified # of steps.
   *
   * @param[in] dt The # of steps to increment by. This is
   * practically unused because the arena state is advanced
   * incrementally at a fixed rate.
   *
   * Also records time in time_ private member.
   * 
   * If `dt == 0`, `return` immediately. Otherwise calls
   * Arena::UpdateEntitiesTimestep() once.
   */
  void AdvanceTime(double dt);

  void AddRobot();

  void AddEntity(EntityType type, int quantity);

  /**
   * @brief
   */
  void AcceptCommand(Communication com);

  /**
   * @brief Reset all entities in Arena.
   */
  void Reset();

  /**
   * @brief Get the Robots in Arena.
   *
   * @return A vector of pointers to the Robots.
   */
  std::vector<class Robot *> get_robots() const { return robots_; }

  /**
   * @brief Under certain circumstance, the compiler requires that the
   * assignment operator is not defined. This `deletes` the default
   * assignment operator.
   */
  Arena &operator=(const Arena &other) = delete;

  /**
   * @brief Under certain circumstance, the compiler requires that the copy
   * constructor is not defined. This `deletes` the default copy constructor.
   */
  Arena(const Arena &other) = delete;

  /**
   * @brief Determine if two entities have collided in the Arena. Collision is
   * defined as the distance between two entities being less than the sum of
   * their radii.
   *
   * @param mobile_e This entity is definitely moving.
   * @param other_e This entity might be mobile or immobile.
   * @param[out] True if entities overlapping.
   *
   **/
  bool IsColliding(
    ArenaMobileEntity * const mobile_e, ArenaEntity * const other_e);

  /**
  * @brief Checks if a robot is within 5 pixels of a food entity object
  *        Then sets robot's flags accordingly.
  **/
  void checkRobotCollideFood(Robot * robot, ArenaEntity * food);

  /**
  * @brief Move the mobile entity to the edge of the other without overlap.
  * Without this, entities tend to get stuck inside one another.
  **/
  void AdjustEntityOverlap(ArenaMobileEntity * const mobile_e,
    ArenaEntity *const other_e);

  /**
   * @brief Determine if a particular entity has gone out of the boundaries of
   * the simulation (i.e. has collided with any one of the walls).
   *
   * @param ent The entity to check.
   * @param [out] An entity type signifying wall (e.g. kRightWall). kUndefined
   * if no collision.
   *
   * The checked entity's position will be updated to a "back-off position" so
   * that it won't get stuck into a wall. The calculation of the "back-off
   * position" is technically not accurate, but good enough for our purpose.
   */
  EntityType GetCollisionWall(ArenaMobileEntity * const ent);

  /**
  * @brief Move the entity to the edge of the wall without overlap.
  * Without this, entities tend to get stuck in walls.
  **/
  void AdjustWallOverlap(ArenaMobileEntity * const ent, EntityType wall);

  /**
   * @brief Update all entities for a single timestep.
   *
   * First calls each entity's TimestepUpdate method to update their speed,
   * heading angle, and position. Then check for collisions between entities
   * or between an entity and a wall.
   * This function also checks for win/loss conditions.
   * Also passes the time_ to the robot by getting current time in robot and 
   * adding the current time so it could calculate invincibility.
   */
  void UpdateEntitiesTimestep();

  std::vector<class ArenaEntity *> get_entities() const { return entities_; }

  double get_x_dim() { return x_dim_; }
  double get_y_dim() { return y_dim_; }

  int get_game_status() const { return game_status_; }
  void set_game_status(int status) { game_status_ = status; }

 private:
  // Dimensions of graphics window inside which entities must operate
  double x_dim_;
  double y_dim_;

  // Used to create all entities within the arena
  EntityFactory *factory_;

  // Robot is special. It's also stored in the entity vectors.
  std::vector<class Robot *> robots_;

  // All the food entities to notify the food sensors of
  std::vector<class Food *> foods_;

  // All entities mobile and immobile.
  std::vector<class ArenaEntity *> entities_;

  // A subset of the entities -- only those that can move (only Robot for now).
  std::vector<class ArenaMobileEntity *> mobile_entities_;

  // A subset of the entities -- only Light objects
  std::vector<class Light *> light_entities_;

  // win/lose/playing state
  int game_status_;

  // current time within the simulation, no getters or setters
  double time_{0.0};
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_H_
