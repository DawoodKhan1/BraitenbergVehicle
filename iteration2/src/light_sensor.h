/**
 * @file light_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_LIGHT_SENSOR_H_
#define SRC_LIGHT_SENSOR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/sensor.h"
#include "src/params.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/**
 * @brief Class representing a Light sensors of the robot
 *
 * Light sensors detect food only when robot is not starving. 
 * 
 * Displayed as white circles on the robot.
 * 
 * Receives position information about Lights from the arena and updates reading.
 */
class LightSensor : public Sensor {
 public:
  LightSensor();

  /**
  * @brief Update reading based on distance and formula
  */
  void Notify(Pose position) override;
};

NAMESPACE_END(csci3081);

#endif  // SRC_LIGHT_SENSOR_H_
