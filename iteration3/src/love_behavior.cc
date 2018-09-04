/**
 * @file love_behavior.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/love_behavior.h"

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
WheelVelocity LoveBehavior::processReading(double leftReading,
    double rightReading) {
  // (-) Direct
  WheelVelocity v;

  v.left = light_max_reading_- leftReading;
  v.right = light_max_reading_ - rightReading;

  return v;
}

NAMESPACE_END(csci3081);
