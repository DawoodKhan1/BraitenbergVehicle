/**
 * @file light_sensor.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <math.h>

#include "src/light_sensor.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
******************************************************************************/
LightSensor::LightSensor() {
  color_.Set(kWhite);
  position_.x = 0;
  position_.y = 0;
  position_.theta = 0;
  reading_ = 0;
}
void LightSensor::Notify(Pose position) {
  reading_ += (1200.0 / (pow(calculateDistance(position), 1.08)));
}

NAMESPACE_END(csci3081);
