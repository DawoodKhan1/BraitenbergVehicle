/**
 * @file Food.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_FOOD_H_
#define SRC_FOOD_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_immobile_entity.h"
#include "src/common.h"
#include "src/entity_type.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a immobile Food within the Arena.
 *
 * Immobile red food objects in the arena. 
 * When a robot is within 5 pixels, the robot's hunger is reset. 
 *
 */
class Food : public ArenaImmobileEntity {
 public:
  /**
   * @brief Constructor.
   *
   * @param params A Food_params passed down from main.cc for the
   * initialization of the Food.
   */
  Food();

  /**
   * @brief Reset the Food using the initialization parameters received
   * by the constructor.
   */
  void Reset() override;

  /**
   * @brief Get the name of the Food for visualization purposes, and to
   * aid in debugging.
   *
   * @return Name of the Food.
   */
  std::string get_name() const override { return "Food"; }

  /**
   * @brief Getter for captured_, which is the state of the Food
   *
   * @return true if captured.
   */
  bool IsCaptured() const { return captured_; }

  /**
   * @brief Setter for captured_, which is the state of the Food
   */
  void set_captured(bool state) { captured_ = state; }

 private:
    bool captured_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_FOOD_H_
