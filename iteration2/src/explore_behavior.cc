/**
 * @file explore_behavior.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/explore_behavior.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
WheelVelocity ExploreBehavior::processReading(double leftReading,
    double rightReading) {
  // (-) crossed
  WheelVelocity v;

  v.left = light_max_reading_ - rightReading;
  v.right = light_max_reading_ - leftReading;

  return v;
}

NAMESPACE_END(csci3081);
