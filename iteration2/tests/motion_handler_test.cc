#include <cmath>
#include <gtest/gtest.h>

#include "../src/motion_handler_robot"
#include "../src/robot.h"
#include "../src/pose.h"

#ifdef MOTIONHANDLER_TESTS

class MotionHandlerRobotTest : public ::testing::Test {
public:
  LightSensorTest(){}

 protected:
  virtual void SetUp() {

     // testing UpdateVelocity function
     csci3081::WheelVelocity w(10.0, 10.0);
     r1.UpdateVelocity(w);

     csci3081::WheelVelocity w2(5.5, 6.5);
     r2.UpdateVelocity(w2);

     csci3081::WheelVelocity w2(4.0, 6.0);
     r3.UpdateVelocity(w2);

     csci3081::WheelVelocity w2(3.5, 1.5);
     r4.UpdateVelocity(w2);
   }

   csci3081::Robot r1;
   csci3081::Robot r2;
   csci3081::Robot r3;
   csci3081::Robot r4;
 };


TESTF(LightSensorTest,AgressiveUpdateVelocity){
  EXPECT_GT(r1.get_velocity().l, 0.0);
  EXPECT_GT(r1.get_velocity().r, 0.0);
}

TESTF(LightSensorTest,LoveUpdateVelocity){
  EXPECT_GT(r2.get_velocity().l, 0.0);
  EXPECT_GT(r2.get_velocity().r, 0.0);
}

TESTF(LightSensorTest,CowardUpdateVelocity){
  EXPECT_GT(r3.get_velocity().l, 0.0);
  EXPECT_GT(r3.get_velocity().r, 0.0);
}

TESTF(LightSensorTest,ExploreUpdateVelocity){
  EXPECT_GT(r4.get_velocity().l, 0.0);
  EXPECT_GT(r4.get_velocity().r, 0.0);
}
#endif
