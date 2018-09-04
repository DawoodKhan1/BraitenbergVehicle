/**
 * @file sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */
#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

/*******************************************************************************
 * Includes
******************************************************************************/
#include <string>
#include <vector>

#include "src/params.h"
#include "src/rgb_color.h"
#include "src/pose.h"

/*******************************************************************************
 * Namespaces
******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class Robot;

/**
 * @brief Parent Class representing a generic sensor of the robot
 *
 * Has useful functions for reading, angle, distance, radius, robot_radius, 
 * etc. that would be useful for all sensors.
 */
class Sensor {
 public:
  Sensor();
  explicit Sensor(double robotRadius);

  Sensor(const Sensor &other) = default;
  Sensor &operator=(const Sensor &other) = default;
  virtual ~Sensor();

  /**
   * @brief Resets sensor to all the initial values
   */
  void Reset();

  /**
  * @brief Update reading based on distance and formula
  */
  virtual void Notify(__unused Pose position);

  void setSensorPositionBasedOnRobotPosition(Pose robotPosition);

  void set_color(RgbColorEnum color) { color_.Set(color); }
  void set_color(RgbColor color);
  RgbColor get_color() { return color_; }

  Pose get_position() { return position_; }
  void set_position(__unused Pose position) { position_ = position; }

  void set_reading(double r) { reading_ = r; }
  double get_reading() { return reading_; }

  double calculateDistance(Pose position);

  double get_angle_offset() const { return angle_offset_; }
  void set_angle_offset(double angle_offset) { angle_offset_ = angle_offset; }

  double get_radius() { return radius_; }
  void set_radius(double radius) { radius_ = radius; }

  double get_robot_radius() { return robot_radius_; }
  void set_robot_radius(double robotRadius) { robot_radius_ = robotRadius; }

 protected:
  RgbColor color_;
  Pose position_;
  double reading_;
  double angle_offset_ = ANGLE_OFFSET;
  double radius_ = 3.0;
  double robot_radius_;  // radius of robot sensor is attached to
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_H_
