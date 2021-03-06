/**
 * @file graphics_arena_viewer.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_GRAPHICS_ARENA_VIEWER_H_
#define SRC_GRAPHICS_ARENA_VIEWER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <MinGfx-1.0/mingfx.h>

#include "src/arena.h"
#include "src/controller.h"
#include "src/common.h"
#include "src/communication.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class Controller;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 *  @brief An application that uses the MinGfx library to open up a window that
 *  includes a few buttons for controlling the simulation and can be used to
 *  draw circles and other computer graphics.
 *
 *  After constructing a new GraphicsArenaViewer, call Run to start and run the
 *  application. Run will not return until the application window is closed.
 *  Example:
 *
 *  ```
 *  int main(int argc, char **argv) {
 *      RobotViewer *app = new RobotViewer();
 *      app->Run();
 *      return 0;
 *  }
 *  ```
 *
 *  Has a pause/play button that starts and stops the simulation.
 *
 *  While the window is open UpdateSimulation will be called repeatedly,
 *  once per frame. It does not advance time if the game is paused.
 *
 *  DrawRobot changes the appearence of the robot depending on the 
 *  hunger level. Also displays 2 white light sensors.
 */
class GraphicsArenaViewer : public GraphicsApp {
 public:
  /**
   * @brief Constructor.
   *
   * @param params A arena_params passed down from main.cc for the
   * initialization of the Arena and the entities therein.
   */
  explicit GraphicsArenaViewer(const struct arena_params *const params,
                               Arena *arena, Controller *controller);

  /**
   * @brief Destructor.
   *
   * `delete` the contained Arena.
   */
  ~GraphicsArenaViewer() override { delete arena_; }

  /**
   * @brief Informs the Arena of the new time, so that it can update.
   *
   * @param dt The new timestep.
   */
  void UpdateSimulation(double dt) override;

  /**
   * @brief Handle the user pressing the pause button on the GUI.
   *
   * This will freeze the graphics--no update, until the pause button is
   * pressed again.
   */
  void OnPlayingBtnPressed();

  /**
   * @brief Draw the Arena with all of its entities using `nanogui`.
   *
   * This is the primary driver for drawing all entities in the Arena. It is
   * called at each iteration of `nanogui::mainloop()`.
   *
   * @param[in] ctx Context for nanogui.
   */
  void DrawUsingNanoVG(NVGcontext *ctx) override;

  /**
   * @brief Draw using `OpenGL`. This method is unimplemented, as currently
   * we are doing all drawing with `nanovg` in this application, so it is empty.
   */
  void DrawUsingOpenGL() override {}

  /**
   * @brief Under certain circumstance, the compiler requires that the
   * assignment operator is not defined. This `deletes` the default
   * assignment operator.
   */
  GraphicsArenaViewer &operator=(const GraphicsArenaViewer &other) = delete;

  /**
   * @brief Under certain circumstance, the compiler requires that the copy
   * constructor is not defined. This `deletes` the default copy constructor.
   */
  GraphicsArenaViewer(const GraphicsArenaViewer &other) = delete;


  /**
   * @brief Getter for paused, which is the pause/playing state of simulation
   *
   * @return true if paused.
   */
  bool IsPaused() const { return paused_; }


  /**
   * @brief Setter for paused_, which is the pause/playing state of simulation
   */
  void set_paused(bool paused) { paused_ = paused; }


 private:
  void DrawArena(NVGcontext *ctx);
  /**
   * @brief Draw a Robot using `nanogui`.
   *
   * This function requires an active `nanovg` drawing context (`ctx`), so it
   * should probably only be called from with DrawUsingNanoVG.
   * 
   * This function also displays the 2 light sensors on the robot
   * 
   * Also checks the state of the robot's hunger and colors the robot
   * depending on the hunger state. 
   * Green - not hungry, yellow - hungry but not starving, 
   * red - starving, purple - starved (dead)
   *
   * @param[in] ctx The `nanovg` context.
   * @param[in] robot The Robot handle.
   */
  void DrawRobot(NVGcontext *ctx, const class Robot *const robot);

  /**
   * @brief Draw an Light in the Arena using `nanogui`.
   *
   * This function requires an active `nanovg` drawing context (`ctx`), so it
   * should probably only be called from with DrawUsingNanoVG.
   *
   * @param[in] ctx The `nanovg` context.
   * @param[in] Light The Light handle.
   */
  void DrawEntity(NVGcontext *ctx, const class ArenaEntity *const entity);

  /**
   * @brief Resets the game upon function call
   *
   * This function is called after clicking the new game button.
   * This calls controller with new game command and then calls
   * Arena Accept Communication which handles resetting the arena.
   *
   */
  void OnNewGameBtnPressed();

  Controller *controller_;
  Arena *arena_;
  // When the game begins, all entities are not moving meaning they should be
  // paused
  bool paused_{true};

  // buttons
  nanogui::Button *playing_button_{nullptr};

  // configuration values
  int robot_fear_count_{N_ROBOTS_FEAR},
      robot_explore_count_{N_ROBOTS_EXPLORE},
      light_count_{N_LightS},
      food_count_{N_FoodS},
      numerator_value_{1200};  // default value in equation for numerator
};

NAMESPACE_END(csci3081);

#endif  // SRC_GRAPHICS_ARENA_VIEWER_H_
