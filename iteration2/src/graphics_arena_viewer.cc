/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <iostream>
#include <string>

#include "src/graphics_arena_viewer.h"
#include "src/arena_params.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
GraphicsArenaViewer::GraphicsArenaViewer(
    const struct arena_params *const params,
    Arena * arena, Controller * controller) :
    GraphicsApp(
        params->x_dim + GUI_MENU_WIDTH + GUI_MENU_GAP * 2,
        params->y_dim,
        "Robot Simulation"),
    controller_(controller),
    arena_(arena) {
  auto *gui = new nanogui::FormHelper(screen());
  nanogui::ref<nanogui::Window> window =
      gui->addWindow(
          Eigen::Vector2i(10 + GUI_MENU_GAP, 10),
          "Menu");

  gui->addGroup("Simulation Control");
  playing_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPlayingBtnPressed, this));
    gui->addButton(
      "New Game",
      std::bind(&GraphicsArenaViewer::OnNewGameBtnPressed, this));
  screen()->performLayout();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
    controller_->AdvanceTime(dt);
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/
void GraphicsArenaViewer::OnPlayingBtnPressed() {
  paused_ = !paused_;

  if (!paused_) {
    playing_button_->setCaption("Pause");
  } else {
    playing_button_->setCaption("Play");
  }
}

/** OnSpecialKeyDown is called when the user presses down on one of the
  * special keys (e.g. the arrow keys).
  */
/**
 * @TODO: Check for arrow key presses using GLFW macros, then
 * convert to appropriate enum Communication and relay to controller
 */
void GraphicsArenaViewer::OnSpecialKeyDown(int key,
  __unused int scancode, __unused int modifiers) {
    Communication key_value = kNone;
    switch (key) {
      case GLFW_KEY_UP:
        key_value = kKeyUp;
        break;
      case GLFW_KEY_DOWN:
        key_value = kKeyDown;
        break;
      case GLFW_KEY_LEFT:
        key_value = kKeyLeft;
        break;
      case GLFW_KEY_RIGHT:
        key_value = kKeyRight;
        break;

      default: {}
    }
  controller_->AcceptCommunication(key_value);
}

/*******************************************************************************
 * Drawing of Entities in Arena
 ******************************************************************************/
void GraphicsArenaViewer::DrawRobot(NVGcontext *ctx,
                                     const Robot *const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading of this robot
  nvgSave(ctx);
  nvgTranslate(ctx,
               static_cast<float>(robot->get_pose().x),
               static_cast<float>(robot->get_pose().y));
  nvgRotate(ctx,
            static_cast<float>(robot->get_pose().theta * M_PI / 180.0));

  // robot's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, static_cast<float>(robot->get_radius()));

  // Color robot depending on hunger state
  // green: not hungry, yellow: hungry but not starving, red: starving
  // purple: robot has starved (died)
  if (!robot->get_hungry())
    nvgFillColor(ctx, nvgRGBA(0, 255, 0, 255));  // green
  else if (robot->get_hungry() && !robot->get_starving())
    nvgFillColor(ctx, nvgRGBA(255, 255, 50, 255));  // yellow
  else if (robot->get_starving() && !robot->get_starved())
    nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));  // red
  else  // otherwise robot is dead and then it is purple
    nvgFillColor(ctx, nvgRGBA(75, 0, 150, 255));

  nvgFill(ctx);

  // robot id text label
  nvgSave(ctx);
  nvgRotate(ctx, static_cast<float>(M_PI / 2.0));

  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgFontSize(ctx, 40.0f);

  nvgRestore(ctx);
  nvgRestore(ctx);

  // left light sensor
  nvgSave(ctx);
  nvgBeginPath(ctx);
  nvgTranslate(ctx,
    static_cast<float>(robot->get_left_lightsensor()->get_position().x),
    static_cast<float>(robot->get_left_lightsensor()->get_position().y));

  nvgCircle(ctx, 0.0, 0.0, robot->get_left_lightsensor()->get_radius());
  nvgFillColor(ctx,
    nvgRGBA(robot->get_left_lightsensor()->get_color().r,
    robot->get_left_lightsensor()->get_color().g,
    robot->get_left_lightsensor()->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(100, 100, 100, 255));
  nvgStroke(ctx);
  nvgRestore(ctx);

  // right light sensor
  nvgSave(ctx);
  nvgBeginPath(ctx);
  nvgTranslate(ctx,
    static_cast<float>(robot->get_right_lightsensor()->get_position().x),
    static_cast<float>(robot->get_right_lightsensor()->get_position().y));

  nvgCircle(ctx, 0.0, 0.0, robot->get_right_lightsensor()->get_radius());
  nvgFillColor(ctx,
                 nvgRGBA(robot->get_right_lightsensor()->get_color().r,
                  robot->get_right_lightsensor()->get_color().g,
                         robot->get_right_lightsensor()->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(100, 100, 100, 255));
  nvgStroke(ctx);
  nvgRestore(ctx);
}
void GraphicsArenaViewer::DrawArena(NVGcontext *ctx) {
  nvgBeginPath(ctx);
  // Creates new rectangle shaped sub-path.
  nvgRect(ctx, 0, 0, arena_->get_x_dim(), arena_->get_y_dim());
  nvgStrokeColor(ctx, nvgRGBA(255, 255, 255, 255));
  nvgStroke(ctx);
}

void GraphicsArenaViewer::DrawEntity(NVGcontext *ctx,
                                       const ArenaEntity *const entity) {
  // Light's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx,
            static_cast<float>(entity->get_pose().x),
            static_cast<float>(entity->get_pose().y),
            static_cast<float>(entity->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(entity->get_color().r, entity->get_color().g,
                       entity->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // Light id text label
  if (entity->get_type() != kRobot) {
    nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgText(ctx,
            static_cast<float>(entity->get_pose().x),
            static_cast<float>(entity->get_pose().y),
            entity->get_name().c_str(), nullptr);
  }
}

void GraphicsArenaViewer::DrawUsingNanoVG(NVGcontext *ctx) {
  // initialize text rendering settings
  nvgFontSize(ctx, 18.0f);
  nvgFontFace(ctx, "sans-bold");
  nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  DrawArena(ctx);
  std::vector<ArenaEntity *> entities = arena_->get_entities();
  for (auto &entity : entities) {
    DrawEntity(ctx, entity);
  } /* for(i..) */
  std::vector<Robot *> robots = arena_->get_robots();
  for (auto &robot : robots) {
    DrawRobot(ctx, robot);
  }

  // if loss display message
  if (arena_->get_game_status() == LOST) {
    nvgFontSize(ctx, 60.0f);
    nvgText(ctx, static_cast<float>(512), static_cast<float>(384),
      "Simulation Over - Robot Has Died!", nullptr);
  }
}

void GraphicsArenaViewer::OnNewGameBtnPressed() {
  playing_button_->setCaption("Play");
  paused_ = true;

  controller_->AcceptCommunication(kNewGame);
}

NAMESPACE_END(csci3081);
