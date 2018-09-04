/**
 * @file params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_PARAMS_H_
#define SRC_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/*
 * @file. Constructors for classes should use reasonable default values as
 * defined here. An exception might be if the value is 0.
 * 
 * @brief This defines common constants for the simulation such as window 
 * size, number of Lights, and game status.
 */

// graphics
#define X_DIM 1024
#define Y_DIM 768
#define TEXT_BOX_WIDTH 50
#define GUI_MENU_WIDTH 180
#define GUI_MENU_GAP 10

// arena
#define N_LightS 4
#define N_ROBOTS_LOVE 0
#define N_ROBOTS_AGRESSIVE 0
#define N_ROBOTS_FEAR 5
#define N_ROBOTS_EXPLORE 5
#define N_ROBOTS (N_ROBOTS_FEAR + N_ROBOTS_EXPLORE + \
  N_ROBOTS_LOVE + N_ROBOTS_AGRESSIVE)
#define MAX_LightS 8
#define ARENA_X_DIM X_DIM
#define ARENA_Y_DIM Y_DIM

// game status
#define WON 0
#define LOST 1
#define PLAYING 2

// entity
#define DEFAULT_POSE \
  { 200, 200, 0}
#define DEFAULT_COLOR \
  { 255, 255, 255 }
#define DEFAULT_RADIUS 20

// mobile entity
#define STARTING_VELOCITY 0.0

// robot
#define ROBOT_ANGLE_DELTA 1
#define ROBOT_SPEED_DELTA 1
#define ROBOT_COLLISION_DELTA 1
#define ROBOT_RADIUS 30
#define ROBOT_MIN_RADIUS 8
#define ROBOT_MAX_RADIUS 14
#define ROBOT_INIT_POS \
  { 500, 500 , 0}
#define ROBOT_COLOR \
  { 0, 0, 255 }

#define ROBOT_HEADING 270
#define ROBOT_INIT_SPEED 0
#define ROBOT_MAX_SPEED 10
#define ROBOT_MAX_ANGLE 360
// offset from which robot's hunger will be reset from a food object
#define PIXEL_OFFSET 5

// Food
#define Food_RADIUS 20
#define Food_COLLISION_DELTA 1
#define Food_INIT_POS \
  { 400, 400 }
#define Food_COLOR \
  { 255, 0, 0 }
#define N_FoodS 4

// Light
#define Light_POSITION \
  { 200, 200 }
#define Light_RADIUS 30
#define Light_MIN_RADIUS 10
#define Light_MAX_RADIUS 50
#define Light_COLOR \
  { 255, 255, 255 }
#define ANGLE_OFFSET 40.0
#define PI 3.14159

#endif  // SRC_PARAMS_H_
