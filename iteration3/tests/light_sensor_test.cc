// @copyright 2018 Dawood Khan
// Google Test Framework
#include <gtest/gtest.h>
#include <cmath>

// Project code from the ../src directory
#include "../src/light_sensor.h"
#include "../src/light.h"
#include "../src/robot.h"
#include "../src/pose.h"

#define HIGH_READING_MIN 50  // high readings should be greater than this
#define MEDIUM_READING_CUTOFF 2  // medium readings should be at least this
/*******************************************************************************
 * Test Cases
 ******************************************************************************/
#ifdef LIGHT_SENSOR_TEST

/************************************************************************
* SETUP
*************************************************************************/

class LightSensorTest : public ::testing::Test {
 public:
  LightSensorTest() {}

 protected:
  virtual void SetUp() {
    closeLight.set_pose(csci3081::Pose(110, 100));
    mediumLight.set_pose(csci3081::Pose(400, 100));
    farLight.set_pose(csci3081::Pose(750, 750));
    // relativeLight will be slightly closer to the rightLightSensor than
    // the leftLightSensor
    relativeLight.set_pose(csci3081::Pose(110, 110));
    // equalLight will have same distance from both sensors
    equalLight.set_pose(csci3081::Pose(100, 105));
    // noLightDistance will have 0 distance from leftLightSensor
    noLightDistance.set_pose(csci3081::Pose(100, 100));

    leftLightSensor.set_position(csci3081::Pose(100, 100));
    // rightLightSensor is slightly below leftLightSensor
    rightLightSensor.set_position(csci3081::Pose(100, 110));

    // set readings to 0 initially
    leftLightSensor.set_reading(0);
    rightLightSensor.set_reading(0);
  }

  csci3081::Light closeLight, mediumLight, farLight, relativeLight,
    equalLight, noLightDistance;
  csci3081::LightSensor leftLightSensor, rightLightSensor;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/
// Default Constructor Test
TEST_F(LightSensorTest, defaultConstructor) {
  csci3081::LightSensor testLightSensor;

  EXPECT_EQ(testLightSensor.get_position().x, 0) <<
    "FAIL: defaultConstructor - Default x position is not 0";
  EXPECT_EQ(testLightSensor.get_position().y, 0) <<
    "FAIL: defaultConstructor - Default y position is not 0";
  EXPECT_EQ(testLightSensor.get_position().theta, 0) <<
    "FAIL: defaultConstructor - Initial theta value is not 0";
  EXPECT_EQ(testLightSensor.get_reading(), 0) <<
    "FAIL: defaultConstructor - Initial reading is not 0";
  EXPECT_EQ(testLightSensor.get_color().r, 255) <<
    "FAIL: defaultConstructor - Color's r value is not 255";
  EXPECT_EQ(testLightSensor.get_color().g, 255) <<
    "FAIL: defaultConstructor - Color's g value is not 255";
  EXPECT_EQ(testLightSensor.get_color().b, 255) <<
    "FAIL: defaultConstructor - Color's b value is not 255";
}

// 1 close light
TEST_F(LightSensorTest, notifyClose) {
  leftLightSensor.Notify(closeLight.get_pose());
  rightLightSensor.Notify(closeLight.get_pose());

  EXPECT_GT(leftLightSensor.get_reading(), HIGH_READING_MIN) <<
    "FAIL: notifyClose - Left Sensor Reading should be at least" <<
    HIGH_READING_MIN;
  EXPECT_GT(rightLightSensor.get_reading(), HIGH_READING_MIN) <<
    "FAIL: notifyClose - Right Sensor Reading should be at least" <<
    HIGH_READING_MIN;

  leftLightSensor.set_reading(0);
  rightLightSensor.set_reading(0);
}

// 1 medium light
TEST_F(LightSensorTest, notifyMedium) {
  leftLightSensor.Notify(mediumLight.get_pose());
  rightLightSensor.Notify(mediumLight.get_pose());

  EXPECT_GT(leftLightSensor.get_reading(), MEDIUM_READING_CUTOFF) <<
    "FAIL: notifyMedium - Left Sensor Reading should be at least"
    << MEDIUM_READING_CUTOFF;
  EXPECT_GT(rightLightSensor.get_reading(), MEDIUM_READING_CUTOFF) <<
    "FAIL: notifyMedium - Right Sensor Reading should be at least"
    << MEDIUM_READING_CUTOFF;

  leftLightSensor.set_reading(0);
  rightLightSensor.set_reading(0);
}

// 1 far light
TEST_F(LightSensorTest, notifyFar) {
  leftLightSensor.Notify(farLight.get_pose());
  rightLightSensor.Notify(farLight.get_pose());

  EXPECT_LT(leftLightSensor.get_reading(), MEDIUM_READING_CUTOFF) <<
    "FAIL: notifyFar - Left Sensor Reading should be at less than"
    << MEDIUM_READING_CUTOFF;
  EXPECT_LT(rightLightSensor.get_reading(), MEDIUM_READING_CUTOFF) <<
    "FAIL: notifyFar - Right Sensor Reading should be less than"
    << MEDIUM_READING_CUTOFF;

  leftLightSensor.set_reading(0);
  rightLightSensor.set_reading(0);
}

// 1 light relatively to the right of the robot
// meaning its closer to the right sensor than the left
TEST_F(LightSensorTest, notifyRelative) {
  leftLightSensor.Notify(relativeLight.get_pose());
  rightLightSensor.Notify(relativeLight.get_pose());

  EXPECT_LT(leftLightSensor.get_reading(), rightLightSensor.get_reading()) <<
    "FAIL: notifyRelative - Left Sensor Reading should be less than Right" <<
    "Sensor Reading";

  leftLightSensor.set_reading(0);
  rightLightSensor.set_reading(0);
}

// 1 light that is equal in distance to both sensors
// so the readings should be equal
TEST_F(LightSensorTest, notifyEqualDistance) {
  leftLightSensor.Notify(equalLight.get_pose());
  rightLightSensor.Notify(equalLight.get_pose());

  EXPECT_EQ(leftLightSensor.get_reading(), rightLightSensor.get_reading()) <<
    "FAIL: notifyEqualDistance - Both Sensor readings should be equal";

  leftLightSensor.set_reading(0);
  rightLightSensor.set_reading(0);
}

// Testing if the light is ontop of the sensor it should still
// register a high reading
TEST_F(LightSensorTest, notifyZeroDistance) {
  leftLightSensor.Notify(noLightDistance.get_pose());

  EXPECT_GT(leftLightSensor.get_reading(), HIGH_READING_MIN) <<
    "FAIL: notifyZeroDistance - Reading should be high";

  leftLightSensor.set_reading(0);
}

// Testing that multiple far lights register as a medium reading
TEST_F(LightSensorTest, notifyMultipleFar) {
  leftLightSensor.Notify(farLight.get_pose());
  leftLightSensor.Notify(farLight.get_pose());
  leftLightSensor.Notify(farLight.get_pose());

  rightLightSensor.Notify(farLight.get_pose());
  rightLightSensor.Notify(farLight.get_pose());
  rightLightSensor.Notify(farLight.get_pose());

  EXPECT_GT(leftLightSensor.get_reading(), MEDIUM_READING_CUTOFF) <<
    "FAIL: notifyFar - Left Sensor Reading should be at least"
    << MEDIUM_READING_CUTOFF;
  EXPECT_GT(rightLightSensor.get_reading(), MEDIUM_READING_CUTOFF) <<
    "FAIL: notifyFar - Right Sensor Reading should be at least"
    << MEDIUM_READING_CUTOFF;

  leftLightSensor.set_reading(0);
  rightLightSensor.set_reading(0);
}

// Testing that MANY medium lights register as a high reading
TEST_F(LightSensorTest, notifyMultipleMedium) {
  for (int i = 0; i < 20; i++)
    leftLightSensor.Notify(mediumLight.get_pose());

  for (int i = 0; i < 20; i++)
    rightLightSensor.Notify(mediumLight.get_pose());

  EXPECT_GT(leftLightSensor.get_reading(), HIGH_READING_MIN) <<
    "FAIL: notifyFar - Left Sensor Reading should be at least"
    << HIGH_READING_MIN;
  EXPECT_GT(rightLightSensor.get_reading(), HIGH_READING_MIN) <<
    "FAIL: notifyFar - Right Sensor Reading should be at least"
    << HIGH_READING_MIN;

  leftLightSensor.set_reading(0);
  rightLightSensor.set_reading(0);
}

// tests numeratorValue setter and getter
TEST_F(LightSensorTest, numeratorValueSetterAndGetter) {
  leftLightSensor.set_numerator_value(10);

  EXPECT_EQ(10, leftLightSensor.get_numerator_value()) <<
    "FAIL: numeratorValueSetterAndGetter - Setter or Getter failed";

    leftLightSensor.set_numerator_value(1200);  // default value
}

// tests angle_offset_ setter and getter
TEST_F(LightSensorTest, angleOffsetSetterAndGetter) {
  leftLightSensor.set_angle_offset(10);

  EXPECT_EQ(10, leftLightSensor.get_angle_offset()) <<
    "FAIL: angleOffsetSetterAndGetter - Setter or Getter failed";

    leftLightSensor.set_angle_offset(40);  // default value
}

// tests calculateDistance function
TEST_F(LightSensorTest, calculateDistance) {
  EXPECT_EQ(leftLightSensor.calculateDistance(
    rightLightSensor.get_position()), 10) <<
    "FAIL: calculateDistance - Distance between sensors is not 10";
}

#endif
