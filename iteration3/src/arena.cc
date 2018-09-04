/**
 * @file arena.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <math.h>
#include <algorithm>
#include <iostream>

#include "src/arena.h"
#include "src/arena_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Arena::Arena(const struct arena_params *const params)
    : x_dim_(params->x_dim),
      y_dim_(params->y_dim),
      factory_(new EntityFactory),
      robots_(),
      foods_(),
      entities_(),
      mobile_entities_(),
      light_entities_(),
      game_status_(PLAYING) {
  AddRobot();
  AddEntity(kFood, params->n_Foods);
  AddEntity(kLight, params->n_Lights);
}

Arena::~Arena() {
  for (auto ent : entities_) {
    delete ent;
  } /* for(ent..) */
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Arena::AddRobot() {
  for (int i = 0; i < N_ROBOTS; i++) {
    Robot * robot_ = dynamic_cast<Robot *>(factory_->CreateEntity(kRobot));
    robots_.push_back(robot_);
    entities_.push_back(robot_);
    mobile_entities_.push_back(robot_);
  }

  // Now set the robot behaviors
  int i = 0;  // index into robot array
  for (; i < N_ROBOTS_FEAR; i++) {
    robots_.at(i)->set_robot_type(kFear);
    robots_.at(i)->set_robot_behavior(new FearBehavior());
  }

  for (; i < N_ROBOTS; i++) {
    robots_.at(i)->set_robot_type(kExplore);
    robots_.at(i)->set_robot_behavior(new ExploreBehavior());
  }
}

void Arena::AddEntity(EntityType type, int quantity) {
  for (int i = 0; i < quantity; i++) {
    ArenaEntity* entity = factory_->CreateEntity(type);
    entities_.push_back(entity);

    if (type == kLight) {  // Lights are now mobile entities
      Light * Light = dynamic_cast<csci3081::Light *>(entity);
      mobile_entities_.push_back(Light);
      light_entities_.push_back(Light);
    } else if (type == kFood) {
      Food * food = dynamic_cast<csci3081::Food *>(entity);
      foods_.push_back(food);
    }
  }
}

void Arena::Reset() {
  for (auto ent : entities_) {
    ent->Reset();
  } /* for(ent..) */
  game_status_ = PLAYING;
} /* reset() */

// The primary driver of simulation movement. Called from the Controller
// but originated from the graphics viewer.
void Arena::AdvanceTime(double dt) {
  time_ = dt;  // record current time

  if (!(dt > 0)) {
    return;
  }
  for (size_t i = 0; i < 1; ++i) {
    UpdateEntitiesTimestep();
  } /* for(i..) */
} /* AdvanceTime() */

void Arena::UpdateEntitiesTimestep() {
  // notify all the sensors within the robots of all the items they
  // are supposed to sense
  for (auto &robot : robots_) {
    for (auto light : light_entities_) {
      robot->get_left_lightsensor()->Notify(light->get_pose());
      robot->get_right_lightsensor()->Notify(light->get_pose());
    }
    for (auto food : foods_) {
      robot->get_left_foodsensor()->Notify(food->get_pose());
      robot->get_right_foodsensor()->Notify(food->get_pose());
    }
  }

  /*
   * First, update the position of all entities, according to their current
   * velocities.
   * @TODO: Should this be just the mobile entities ??
   */
  for (auto ent : entities_) {
    ent->TimestepUpdate(1);
  }

  for (auto ent : entities_) {
    if (ent->get_type() == kLight) {
      Light * Light = dynamic_cast<csci3081::Light *>(ent);
      Light->set_time(Light->get_time() + time_);
    }
  }

  // Check if any robots are starved
  for (auto &robot : robots_) {
    if (robot->get_starved())
      game_status_ = LOST;   // loss is indicated by game_status_ == 1
  }

   /* Determine if any mobile entity is colliding with wall.
   * Adjust the position accordingly so it doesn't overlap.
   */
  for (auto &ent1 : mobile_entities_) {
    EntityType wall = GetCollisionWall(ent1);
    if (kUndefined != wall) {
      AdjustWallOverlap(ent1, wall);
      if (ent1->get_type() == kRobot) {
        Robot * robot = dynamic_cast<Robot *>(ent1);
        robot->HandleCollision(wall);
      } else if (ent1->get_type() == kLight) {
        // Light* Light = ;
        dynamic_cast<Light *>(ent1)->HandleCollision(wall);
      }
    }
    /* Determine if that mobile entity is colliding with any other entity.
    * Adjust the position accordingly so they don't overlap.
    */
    for (auto &ent2 : entities_) {
      if (ent2 == ent1) { continue; }
      if (ent1->get_type() == kRobot && ent2->get_type() == kFood) {
        // if robot is within 5 pixels (distance) of a food object, hunger
        // should be reset
        Robot * robot = dynamic_cast<Robot *>(ent1);
        checkRobotCollideFood(robot, ent2);
      }
      if (IsColliding(ent1, ent2)) {
        AdjustEntityOverlap(ent1, ent2);
        if (ent1->get_type() == kRobot) {
          Robot * robot = dynamic_cast<Robot *>(ent1);
          robot->HandleCollision(ent2->get_type(), ent2);
        } else if (ent1->get_type() == kLight) {
          dynamic_cast<Light *>(ent1)->
            HandleCollision(ent2->get_type(), ent2);
        }
      }
    }
  }
}  // UpdateEntitiesTimestep()

void Arena::checkRobotCollideFood(Robot * robot, ArenaEntity * food) {
  Pose robotPos = robot->get_pose();
  Pose foodPos = food->get_pose();

  double distance = sqrt(pow(robotPos.x - foodPos.x, 2) +
    pow(robotPos.y - foodPos.y, 2));

  if (distance <= robot->get_radius() + food->get_radius() + PIXEL_OFFSET) {
    robot->set_hungry(false);
    robot->set_starving(false);
  }
}  // checkRobotCollideFood()
// Determine if the entity is colliding with a wall.
// Always returns an entity type. If not collision, returns kUndefined.
EntityType Arena::GetCollisionWall(ArenaMobileEntity *const ent) {
  if (ent->get_pose().x + ent->get_radius() >= x_dim_) {
    return kRightWall;  // at x = x_dim_
  } else if (ent->get_pose().x - ent->get_radius() <= 0) {
    return kLeftWall;  // at x = 0
  } else if (ent->get_pose().y + ent->get_radius() >= y_dim_) {
    return kBottomWall;  // at y = y_dim
  } else if (ent->get_pose().y - ent->get_radius() <= 0) {
    return kTopWall;  // at y = 0
  } else {
    return kUndefined;
  }
}  /* GetCollisionWall() */

/* The entity type indicates which wall the entity is colliding with.
* This determines which way to move the entity to set it slightly off the wall. */
void Arena::AdjustWallOverlap(ArenaMobileEntity *const ent, EntityType object) {
  Pose entity_pos = ent->get_pose();
  switch (object) {
    case (kRightWall):  // at x = x_dim_
    ent->set_position(x_dim_-(ent->get_radius()+5), entity_pos.y);
    break;
    case (kLeftWall):  // at x = 0
    ent->set_position(ent->get_radius()+5, entity_pos.y);
    break;
    case (kTopWall):  // at y = 0
    ent->set_position(entity_pos.x, ent->get_radius()+5);
    break;
    case (kBottomWall):  // at y = y_dim_
    ent->set_position(entity_pos.x, y_dim_-(ent->get_radius()+5));
    break;
    default:
    {}
  }
}

/* Calculates the distance between the center points to determine overlap */
bool Arena::IsColliding(
  ArenaMobileEntity * const mobile_e,
  ArenaEntity * const other_e) {
    double delta_x = other_e->get_pose().x - mobile_e->get_pose().x;
    double delta_y = other_e->get_pose().y - mobile_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    return
    (distance_between <= (mobile_e->get_radius() + other_e->get_radius()));
}

/* This is called when it is known that the two entities overlap.
* We determine by how much they overlap then move the mobile entity to
* the edge of the other
*/
/* @TODO: Add functionality to Pose to determine the distance distance_between
   two instances (e.g. overload operator -) */
/* @BUG: The robot will pass through the home Food on occasion. The problem
 * is likely due to the adjustment being in the wrong direction. This could
 * be because the cos/sin generate the wrong sign of the distance_to_move
 * when the collision is in a specific quadrant relative to the center of the
 * colliding entities..
 */
void Arena::AdjustEntityOverlap(ArenaMobileEntity * const mobile_e,
  ArenaEntity *const other_e) {
    bool adjustOverlap = true;  // whether to adjust overlap or not
    /* Lights only collide with other lights and the wall,
       Robots only collide with other robots and the wall */
    if (mobile_e->get_type() == kLight && !(other_e->get_type() == kLight ||
        other_e->get_type() == kTopWall ||
        other_e->get_type() == kBottomWall ||
        other_e->get_type() == kLeftWall ||
        other_e->get_type() == kRightWall)) {
      adjustOverlap = false;
    } else if (mobile_e->get_type() == kRobot &&
        !(other_e->get_type() == kRobot ||
        other_e->get_type() == kTopWall ||
        other_e->get_type() == kBottomWall ||
        other_e->get_type() == kLeftWall ||
        other_e->get_type() == kRightWall)) {
      adjustOverlap = false;
    }

    if (adjustOverlap) {
      double delta_x = mobile_e->get_pose().x - other_e->get_pose().x;
      double delta_y = mobile_e->get_pose().y - other_e->get_pose().y;
      double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
      double distance_to_move =
        mobile_e->get_radius() + other_e->get_radius() - distance_between + 5;
      double angle = atan2(delta_y, delta_x);
      mobile_e->set_position(
        mobile_e->get_pose().x+cos(angle)*distance_to_move,
        mobile_e->get_pose().y+sin(angle)*distance_to_move);
    }
}

// Accept communication from the controller. Dispatching as appropriate.
/** @TODO: Call the appropriate Robot functions to implement user input
  * for controlling the robot.
  */
void Arena::AcceptCommand(Communication com) {
  switch (com) {
    case(kIncreaseSpeed):
      break;
    case(kDecreaseSpeed):
      break;
    case(kTurnLeft):
      break;
    case(kTurnRight):
      break;
    case(kPlay):
    case(kPause):
    case(kReset): {
      Reset();
    }
    case(kNone):
    default: break;
  }
} /* AcceptCommand */

void Arena::AcceptGUIParameters(int robotFearCount, int robotExploreCount,
      int lightCount, int foodCount, int numeratorValue) {
  for (auto &robot : robots_) {
    robot->get_left_lightsensor()->set_numerator_value(numeratorValue);
    robot->get_right_lightsensor()->set_numerator_value(numeratorValue);
  }

  // make all robots ignore hunger it simulation is not already over
  if (foodCount == 0 && game_status_ != LOST) {
    for (auto &robot : robots_) {
      robot->set_time_since_last_meal(0);
      robot->set_hungry(false);
      robot->set_starving(false);
      robot->set_starved(false);
      robot->set_ignore_hunger(true);
    }
  } else {
    for (auto &robot : robots_) {  // do not ignore hunger if food is in arena
      robot->set_ignore_hunger(false);
    }
  }

  // update lights in arena
  if (static_cast<unsigned int>(lightCount) > light_entities_.size()) {
    // add lights
    int numAddLights = lightCount - light_entities_.size();

    AddEntity(kLight, numAddLights);
  } else if (static_cast<unsigned int>(lightCount) < light_entities_.size()) {
    // remove lights
    int numRemoveLights = light_entities_.size() - lightCount;

    for (int i = 0; i < numRemoveLights; i++) {
      // want to remove the object from all vectors and
      // delete the object itself
      bool removed = false;  // stop the loops when element has been removed

      Light * light = light_entities_.front();  // light to remove, keep ptr
      light_entities_.erase(light_entities_.begin());

      // remove this light from entities and mobileentities vector
      for (unsigned int j = 0; j < entities_.size() && !removed; j++)
        if (entities_[j] == light) {
          entities_.erase(entities_.begin() + j);  // begin starts at 0
          removed = true;
        }

      removed = false;
      for (unsigned int j = 0; j < mobile_entities_.size() && !removed; j++)
        if (mobile_entities_[j] == light) {
          mobile_entities_.erase(mobile_entities_.begin() + j);
          removed = true;
        }
      // after removing object from all vectors delete the object itself
      delete light;
    }
  }

  // update foods in arena
  if (static_cast<unsigned int>(foodCount) > foods_.size()) {
    // add foods
    int numAddFoods = foodCount - foods_.size();

    AddEntity(kFood, numAddFoods);
  } else if (static_cast<unsigned int>(foodCount) < light_entities_.size()) {
     // remove foods
    int numRemoveFoods = foods_.size() - foodCount;

    for (int i = 0; i < numRemoveFoods; i++) {
      // want to remove the object from all vectors and
      // delete the object itself
      bool removed = false;  // stop the loops when element has been removed

      Food * food = foods_.front();  // food to remove, keep ptr
      foods_.erase(foods_.begin());

      // remove this food from entities and mobileentities vector
      for (unsigned int j = 0; j < entities_.size() && !removed; j++)
        if (entities_[j] == food) {
          entities_.erase(entities_.begin() + j);  // begin starts at 0
          removed = true;
        }

      removed = false;
      for (unsigned int j = 0; j < mobile_entities_.size() && !removed; j++)
        if (mobile_entities_[j] ==
            dynamic_cast<csci3081::ArenaMobileEntity *>(food)) {
          mobile_entities_.erase(mobile_entities_.begin() + j);
          removed = true;
        }
      // after removing object from all vectors delete the object itself
      delete food;
    }
  }
  // update robots in arena
  int numFearRobots = 0,
      numExploreRobots = 0;
  for (auto robot : robots_)
    if (robot->get_robot_type() == kFear)
      numFearRobots++;
    else if (robot->get_robot_type() == kExplore)
      numExploreRobots++;

  // update fear robots
  if (numFearRobots > robotFearCount) {  // delete extra fear robots
    for (int i = 0; i < numFearRobots - robotFearCount; i++) {
      // want to remove the object from all vectors and
      // delete the object itself
      bool removed = false;  // stop the loops when element has been removed

      Robot *fearRobot = NULL;  // get pointer to fear robot
      for (unsigned int j = 0; j < robots_.size(); j++)
        if (robots_[j]->get_robot_type() == kFear)
          fearRobot = robots_[j];

      // remove this fear robot from robots, entities,
        // and mobileentities vectors
      for (unsigned int j = 0; j < robots_.size() && !removed; j++)
        if (robots_[j] == fearRobot) {
          robots_.erase(robots_.begin() + j);  // begin starts at 0
          removed = true;
        }

      removed = false;
      for (unsigned int j = 0; j < entities_.size() && !removed; j++)
        if (entities_[j] == fearRobot) {
          entities_.erase(entities_.begin() + j);  // begin starts at 0
          removed = true;
        }

      removed = false;
      for (unsigned int j = 0; j < mobile_entities_.size() && !removed; j++)
        if (mobile_entities_[j] ==
            dynamic_cast<csci3081::ArenaMobileEntity *>(fearRobot)) {
          mobile_entities_.erase(mobile_entities_.begin() + j);
          removed = true;
        }

      delete fearRobot->get_left_lightsensor();
      delete fearRobot->get_right_lightsensor();
      delete fearRobot->get_left_foodsensor();
      delete fearRobot->get_right_foodsensor();
      delete fearRobot;
    }
  } else if (numFearRobots < robotFearCount) {  // need to add robots
    int numAddFearRobots = robotFearCount - numFearRobots;

    for (int i = 0; i < numAddFearRobots; i++) {
      ArenaEntity* entity = factory_->CreateEntity(kRobot);
      entities_.push_back(entity);

      Robot * fearRobot = dynamic_cast<csci3081::Robot *>(entity);
      mobile_entities_.push_back(fearRobot);
      robots_.push_back(fearRobot);
    }
  }

  // update explore robots
  if (numExploreRobots > robotExploreCount) {  // delete extra Explore robots
    for (int i = 0; i < numExploreRobots - robotExploreCount; i++) {
      // want to remove the object from all vectors and
      // delete the object itself
      bool removed = false;  // stop the loops when element has been removed

      Robot *ExploreRobot = NULL;  // get pointer to Explore robot
      for (unsigned int j = 0; j < robots_.size(); j++)
        if (robots_[j]->get_robot_type() == kExplore)
          ExploreRobot = robots_[j];

      // remove this Explore robot from robots, entities,
      // and mobileentities vectors
      for (unsigned int j = 0; j < robots_.size() && !removed; j++)
        if (robots_[j] == ExploreRobot) {
          robots_.erase(robots_.begin() + j);
          removed = true;
        }

      removed = false;
      for (unsigned int j = 0; j < entities_.size() && !removed; j++)
        if (entities_[j] == ExploreRobot) {
          entities_.erase(entities_.begin() + j);
          removed = true;
        }

      removed = false;
      for (unsigned int j = 0; j < mobile_entities_.size() && !removed; j++)
        if (mobile_entities_[j] ==
            dynamic_cast<csci3081::ArenaMobileEntity *>(ExploreRobot)) {
          mobile_entities_.erase(mobile_entities_.begin() + j);
          removed = true;
        }

      delete ExploreRobot->get_left_lightsensor();
      delete ExploreRobot->get_right_lightsensor();
      delete ExploreRobot->get_left_foodsensor();
      delete ExploreRobot->get_right_foodsensor();
      delete ExploreRobot;
    }
  } else if (numExploreRobots < robotExploreCount) {  // need to add robots
    int numAddExploreRobots = robotExploreCount - numExploreRobots;

    for (int i = 0; i < numAddExploreRobots; i++) {
      ArenaEntity* entity = factory_->CreateEntity(kRobot);
      entities_.push_back(entity);

      Robot * exploreRobot = dynamic_cast<csci3081::Robot *>(entity);
      mobile_entities_.push_back(exploreRobot);
      robots_.push_back(exploreRobot);
    }
  }
}

NAMESPACE_END(csci3081);
