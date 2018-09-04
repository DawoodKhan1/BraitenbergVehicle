/**
 * @file agressive_behavior.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/agressive_behavior.h"

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
WheelVelocity AgressiveBehavior::processReading(double leftReading,
    double rightReading) {
  // (+) Crossed
  WheelVelocity v;

  v.left = rightReading;
  v.right = leftReading;

  return v;
}

NAMESPACE_END(csci3081);
