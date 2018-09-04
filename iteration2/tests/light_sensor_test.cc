const double MAX_READING = 1000.0;

#include <cmath>
#include <gtest/gtest.h>

#include "../src/light_sensor.h"
#include "../src/light.h"
#include "../src/robot.h"
#include "../src/pose.h"

#ifdef LIGHTSENSOR_TESTS

class LightSensorTest : public ::testing::Test {
public:
  LightSensorTest(){}

 protected:
  virtual void SetUp() {

     // initializing l2 and then resetting and testing the Reset() method
     l2.set_reading(10.0);
     csci3081::RgbColor color;
     color.r = 255;
     color.g = 255;
     color.b = 255;

     l2.set_color(color);
     l2.set_robotPtr(&r);
     l2.Reset();

     // make a light sensor close to the robot
     l3.set_robotPtr(&r);
     csci3081::Pose p(100,100);
     r.set_pose(p);
     l3.Notify(csci3081::Pose(200, 100));

     // far away light sensor
     r1.set_pose(csci3081::Pose(0,0));
     l4.set_robotPtr(&r1);
     l4.Notify(csci3081::Pose(1024, 768));

     // test for max intensity reading
     r2.set_pose(csci3081::Pose(200,200));
     l5.set_robotPtr(&r2);
     l5.Notify(csci3081::Pose(200,200));

     // testing multiple notify()
     l6.set_robotPtr(&r2);
     l6.Notify(csci3081::Pose(300,200));
     l6.Notify(csci3081::Pose(200,300));
     l6.Notify(csci3081::Pose(250,200));
   }
  /* declare members */
   csci3081::LightSensor l1;      // default constructor
   csci3081::LightSensor l2;        // Initialized light sensor
   csci3081::LightSensor l3;        // close light sensor
   csci3081::LightSensor l4;        // far away light sensor
   csci3081::LightSensor l5;        // for an extremely close Light
   csci3081::LightSensor l6;        // testing multiple notify()

   csci3081::Robot r;
   csci3081::Robot r1;
   csci3081::Robot r2;
 };

 TEST_F(LightSensorTest,DefaultConstructor){
   EXPECT_EQ(l1.robotPtr_, NULL) << "FAIL: Robot Pointer not NULL.";
   EXPECT_EQ(l1.reading_, NULL) << "FAIL: Reading not 0.";
   EXPECT_EQ(l1.color_.r, 0) << "FAIL: Color red value not 0";
   EXPECT_EQ(l1.color_.g, 0) << "FAIL: Color green value not 0";
   EXPECT_EQ(l1.color_.b, 0) << "FAIL: Color blue value not 0";
}

TESTF(LightSensorTest,Reset){
  EXPECT_EQ(l1.robotPtr_, NULL) << "FAIL: Robot Pointer not NULL.";
  EXPECT_EQ(l1.reading_, NULL) << "FAIL: Reading not 0.";
  EXPECT_EQ(l1.color_.r, 0) << "FAIL: Color red value not 0";
  EXPECT_EQ(l1.color_.g, 0) << "FAIL: Color green value not 0";
  EXPECT_EQ(l1.color_.b, 0) << "FAIL: Color blue value not 0";
}

TESTF(LightSensorTest,Notify){
  EXPECT_LT(l3.reading_, 0) << "FAIL: Reading did not change from close sensor.";
}

TESTF(LightSensorTest,NotifyMinReading){
  EXPECT_EQ(l4.reading_, 0.0) << "FAIL: Reading did not change from far away sensor.";
}

TESTF(LightSensorTest,NotifyMaxReading){
  EXPECT_EQ(l5.get_reading(), MAX_READING) << "FAIL: Reading did not change from extremely close sensor.";
}

TESTF(LightSensorTest,MultipleNotify){
  EXPECT_GT(l6.reading_, 0) << "FAIL: Reading did not change from multiple Lights.";
}

TESTF(LightSensorTest,UpdateVelocity){
  EXPECT_EQ(r2.get_velocity().l, 10.0);
  EXPECT_EQ(r2.get_velocity().r, 10.0);
  EXPECT_EQ(r3.get_velocity().l, 5.5);
  EXPECT_EQ(r3.get_velocity().r, 6.5);
}

#endif
