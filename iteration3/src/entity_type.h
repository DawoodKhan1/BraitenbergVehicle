/**
 * @file entity_type.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ENTITY_TYPE_H_
#define SRC_ENTITY_TYPE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/**
 * @brief Class representing a immobile Food within the Arena.
 *
 * EntityType is used during Lights to determine correct behavior.
 * 
 * For example, if a robot hits a wall or Light then it should decrease 
 * a life.
 */

enum EntityType {
  kRobot, kLight, kFood, kEntity,
  kRightWall, kLeftWall, kTopWall, kBottomWall,
  kUndefined
};

NAMESPACE_END(csci3081);

#endif  // SRC_ENTITY_TYPE_H_
