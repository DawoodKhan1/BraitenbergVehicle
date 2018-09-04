/**
 * @file sensor.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
******************************************************************************/
#include <math.h>

#include "src/params.h"
#include "src/sensor.h"

/*******************************************************************************
 * Namespaces
******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
******************************************************************************/
Sensor::Sensor() : color_(0, 0, 0), position_(), reading_(0),
  robot_radius_(ROBOT_RADIUS) {}

Sensor::Sensor(double robotRadius) :
  color_(0, 0, 0), position_(), reading_(0), robot_radius_(robotRadius) {}

Sensor::~Sensor() {}

void Sensor::Reset() {
}

void Sensor::set_color(RgbColor color) {
  color_.r = color.r;
  color_.g = color.g;
  color_.b = color.b;
}

void Sensor::Notify(__unused Pose position) {
}

double Sensor::calculateDistance(Pose position) {
  return sqrt(pow(position.x - position_.x, 2) +
    pow(position.y - position_.y, 2));
}

void Sensor::setSensorPositionBasedOnRobotPosition(Pose robotPosition) {
  position_.x = robotPosition.x + robot_radius_ *
    cos((robotPosition.theta + angle_offset_) * (PI / 180));
  position_.y = robotPosition.y + robot_radius_ *
    sin((robotPosition.theta + angle_offset_) * (PI / 180));
}

NAMESPACE_END(csci3081);
