/**
 * @file Food.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/food.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Food::Food() : ArenaImmobileEntity(), captured_(false) {
  set_type(kFood);
  set_color(Food_COLOR);
  set_pose(Food_INIT_POS);
  set_radius(Food_RADIUS);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Food::Reset() {
  set_pose(SetPoseRandomly());
  set_color(Food_COLOR);
  captured_ = false;
} /* Reset */

NAMESPACE_END(csci3081);
