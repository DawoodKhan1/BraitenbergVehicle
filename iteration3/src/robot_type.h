/**
 * @file robot_type.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_TYPE_H_
#define SRC_ROBOT_TYPE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/**
 * @brief Class representing the different type of robots.
 *
 * RobotType is used for robots to determine correct behavior.
 * 
 * For example, if a robot is of type kFear then it will avoid lights
 */

enum RobotType {
  kFear,  // Coward
  kAgressive,
  kLove,
  kExplore
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_TYPE_H_
