/**
 * @file entity_factory.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <ctime>
#include <iostream>

#include "src/common.h"
#include "src/entity_factory.h"
#include "src/entity_type.h"
#include "src/params.h"
#include "src/pose.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

EntityFactory::EntityFactory() {
  srand(time(nullptr));
}

ArenaEntity* EntityFactory::CreateEntity(EntityType etype) {
  switch (etype) {
    case (kRobot):
      return CreateRobot();
      break;
    case (kLight):
      return CreateLight();
      break;
    case (kFood):
      return CreateFood();
      break;
    default:
      std::cout << "FATAL: Bad entity type on creation\n";
      assert(false);
  }
  return nullptr;
}

Robot* EntityFactory::CreateRobot() {
  auto* robot = new Robot;
  robot->set_type(kRobot);
  robot->set_color(ROBOT_COLOR);
  robot->set_pose(SetPoseRandomly());

  double rand_radius = random() %
    (ROBOT_MAX_RADIUS - ROBOT_MIN_RADIUS + 1) + ROBOT_MIN_RADIUS;
  robot->set_radius(rand_radius);

  ++entity_count_;
  ++robot_count_;
  robot->set_id(robot_count_);

  // setting the radius of the robot so the sensors can be on it
  robot->get_left_lightsensor()->set_robot_radius(robot->get_radius());
  robot->get_right_lightsensor()->set_robot_radius(robot->get_radius());
  robot->get_left_foodsensor()->set_robot_radius(robot->get_radius());
  robot->get_right_foodsensor()->set_robot_radius(robot->get_radius());

    // Update sensors position based on the robot's position
  robot->get_left_lightsensor()->setSensorPositionBasedOnRobotPosition(
    robot->get_pose());
  robot->get_right_lightsensor()->setSensorPositionBasedOnRobotPosition(
    robot->get_pose());
  robot->get_left_lightsensor()->setSensorPositionBasedOnRobotPosition(
    robot->get_pose());
  robot->get_right_lightsensor()->setSensorPositionBasedOnRobotPosition(
    robot->get_pose());

  return robot;
}

Light* EntityFactory::CreateLight() {
  auto* Light = new csci3081::Light;
  Light->set_type(kLight);
  Light->set_color(Light_COLOR);
  Light->set_pose(SetPoseRandomly());

  double rand_radius = random() %
    (Light_MAX_RADIUS - Light_MIN_RADIUS + 1) + Light_MIN_RADIUS;

  Light->set_radius(rand_radius);
  ++entity_count_;
  ++Light_count_;
  Light->set_id(Light_count_);
  return Light;
}

Food* EntityFactory::CreateFood() {
  auto* Food = new csci3081::Food;
  Food->set_type(kFood);
  Food->set_color(Food_COLOR);
  Food->set_pose(SetPoseRandomly());
  Food->set_radius(Food_RADIUS);
  ++entity_count_;
  ++Food_count_;
  Food->set_id(Food_count_);
  return Food;
}

Pose EntityFactory::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  return {static_cast<double>((30 + (random() % 19) * 50)),
        static_cast<double>((30 + (random() % 14) * 50))};
}

NAMESPACE_END(csci3081);
