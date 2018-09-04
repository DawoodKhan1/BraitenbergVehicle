/**
 * @file food_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_FOOD_SENSOR_H_
#define SRC_FOOD_SENSOR_H_

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
 * @brief Class representing a Food sensors of the robot
 *
 * Food sensors detect food only when hungry. Food sensors are not displayed.
 *
 * Receives position information about Foods from the arena and updates reading.
 */
class FoodSensor : public Sensor {
 public:
  FoodSensor();

  /**
  * @brief Update reading based on distance and formula
  */
  void Notify(Pose position) override;

 private:
  double base_{1.08};
};

NAMESPACE_END(csci3081);

#endif  // SRC_FOOD_SENSOR_H_
