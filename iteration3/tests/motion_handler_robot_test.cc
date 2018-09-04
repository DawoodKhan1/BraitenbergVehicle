// @copyright 2018 Dawood Khan
// Google Test Framework
#include <gtest/gtest.h>
#include <cmath>
#include <iostream>
// Project code from the ../src directory
#include "../src/motion_handler_robot.h"
#include "../src/robot.h"
#include "../src/pose.h"
#include "../src/light.h"
#include "../src/robot_type.h"
#include "../src/robot_behavior.h"
#include "../src/agressive_behavior.h"
#include "../src/love_behavior.h"
#include "../src/explore_behavior.h"
#include "../src/fear_behavior.h"

#ifdef MOTIONHANDLER_TESTS

class MotionHandlerRobotTest : public ::testing::Test {
 public:
  MotionHandlerRobotTest() {}

 protected:
  virtual void SetUp() {
    // setting up robot behaviors
    fearRobot.set_robot_type(csci3081::kFear);
    fearRobot.set_robot_behavior(new csci3081::FearBehavior());

    exploreRobot.set_robot_type(csci3081::kExplore);
    exploreRobot.set_robot_behavior(new csci3081::ExploreBehavior());

    loveRobot.set_robot_type(csci3081::kLove);
    loveRobot.set_robot_behavior(new csci3081::LoveBehavior());

    agressiveRobot.set_robot_type(csci3081::kAgressive);
    agressiveRobot.set_robot_behavior(new csci3081::AgressiveBehavior());

     // testing UpdateVelocity function
     /*csci3081::WheelVelocity w(10.0, 10.0);
     fearRobot.UpdateVelocity(w);

     csci3081::WheelVelocity w2(5.5, 6.5);
     fearRobot.UpdateVelocity(w2);

     csci3081::WheelVelocity w2(4.0, 6.0);
     fearRobot.UpdateVelocity(w2);

     csci3081::WheelVelocity w2(3.5, 1.5);
     fearRobot.UpdateVelocity(w2);*/
  }

  csci3081::Robot fearRobot;
  csci3081::Robot exploreRobot;
  csci3081::Robot loveRobot;
  csci3081::Robot agressiveRobot;
};

// fear robot should have no velocity if there are no lights
TEST_F(MotionHandlerRobotTest, FearUpdateVelocityNoLight) {
  csci3081::WheelVelocity w = fearRobot.get_robot_behavior()->processReading(
  fearRobot.get_left_lightsensor()->get_reading(),
  fearRobot.get_right_lightsensor()->get_reading());

  fearRobot.get_motion_handler().UpdateVelocity(w);

  EXPECT_EQ(fearRobot.get_motion_handler().get_velocity().left, 0.0) <<
    "FAIL: FearUpdateVelocityNoLight - FearRobot's left velocity isn't 0";
  EXPECT_EQ(fearRobot.get_motion_handler().get_velocity().right, 0.0) <<
    "FAIL: FearUpdateVelocityNoLight - FearRobot's right velocity isn't 0";
}

// if there are no lights explore robot should be moving at max speed
// which is 10
TEST_F(MotionHandlerRobotTest, ExploreUpdateVelocityNoLight) {
  csci3081::WheelVelocity w =
    exploreRobot.get_robot_behavior()->processReading(0, 0);

  exploreRobot.get_motion_handler().UpdateVelocity(w);


  EXPECT_EQ(exploreRobot.get_motion_handler().get_velocity().left, 10) <<
    "FAIL: FearUpdateVelocityNoLight - ExploreRobot's left velocity isn't 10";
  EXPECT_EQ(exploreRobot.get_motion_handler().get_velocity().right, 10) <<
    "FAIL: FearUpdateVelocityNoLight - ExploreRobot's right velocity isn't 10";
}
/*
TEST_F(MotionHandlerRobotTest, LoveUpdateVelocityNoLight){
  EXPECT_GT(loveRobot.get_velocity().l, 0.0);
  EXPECT_GT(loveRobot.get_velocity().r, 0.0);
}


TEST_F(MotionHandlerRobotTest, AgressiveUpdateVelocityNoLight){
  EXPECT_GT(agressiveRobot.get_velocity().l, 0.0);
  EXPECT_GT(agressiveRobot.get_velocity().r, 0.0);
}*/




#endif
