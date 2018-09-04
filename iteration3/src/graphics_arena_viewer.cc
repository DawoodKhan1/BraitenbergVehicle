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

  // vvvvvvvvvvvv    ADDED THIS ONE LINE to register the window  vvvvvvvvvvvv
  // gui->addGroup creates a heading within the window
  window->setLayout(new nanogui::GroupLayout());

  // This code was here before
  gui->addGroup("Simulation Control");
  playing_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPlayingBtnPressed, this));
    gui->addButton(
      "New Game",
      std::bind(&GraphicsArenaViewer::OnNewGameBtnPressed, this));

  // Without fixing the width, the button will span the entire window
  playing_button_->setFixedWidth(100);

  // vvvvvvvvvv  ADDED BELOW HERE (from nanogui example1.cc)   vvvvvvvvvvvvvvv

  gui->addGroup("Arena Configuration");

  // Creating a panel impacts the layout. Widgets, sliders, buttons can be
  // assigned to either the window or the panel.
  nanogui::Widget *panel = new nanogui::Widget(window);

  // *************** SLIDER 1 ************************//
  new nanogui::Label(panel, "Number of Fear Robots", "sans-bold");
  nanogui::Slider *slider = new nanogui::Slider(panel);
  // The starting value (range is from 0 to 1)
  // Note that below the displayed value is 10* slider value.
  slider->setValue(1.0f);
  slider->setFixedWidth(100);

  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *textBox = new nanogui::TextBox(panel);
  textBox->setFixedSize(nanogui::Vector2i(60, 25));
  textBox->setFontSize(20);
  textBox->setValue("5");

  // This is the lambda function called while the user is moving the slider
  slider->setCallback(
    [textBox](float value) {
      textBox->setValue(std::to_string(static_cast<int>(value*5)));
    });
  // This is the lambda function called once the user is no longer
  // manipulating the slider. Note robot_fear_count_ is set, which
  // is a graphics_arena_ variable in this version, although
  // you should communicate that value to the controller so that it
  // can configure the Arena.
  slider->setFinalCallback(
    [&](float value) {
      robot_fear_count_ = static_cast<int>(value*5);
      controller_->AcceptGUIParameters(robot_fear_count_, robot_explore_count_,
        light_count_, food_count_, numerator_value_);
    });

  // *************** SLIDER 2 ************************//
  new nanogui::Label(panel, "Number of Explore Robots", "sans-bold");
  nanogui::Slider *slider2 = new nanogui::Slider(panel);
  slider2->setValue(1.0f);
  slider2->setFixedWidth(100);

  nanogui::TextBox *textBox2 = new nanogui::TextBox(panel);
  textBox2->setFixedSize(nanogui::Vector2i(60, 25));
  textBox2->setFontSize(20);
  textBox2->setValue("5");

  slider2->setCallback(
    [textBox2](float value) {
      textBox2->setValue(std::to_string(static_cast<int>(value*5)));
    });

  slider2->setFinalCallback(
    [&](float value) {
      robot_explore_count_ = static_cast<int>(value*5);
      controller_->AcceptGUIParameters(robot_fear_count_, robot_explore_count_,
        light_count_, food_count_, numerator_value_);
    });

  // *************** SLIDER 3 ************************//
  new nanogui::Label(panel, "Number of Lights", "sans-bold");
  nanogui::Slider *slider3 = new nanogui::Slider(panel);
  slider3->setValue(0.4f);
  slider3->setFixedWidth(100);

  nanogui::TextBox *textBox3 = new nanogui::TextBox(panel);
  textBox3->setFixedSize(nanogui::Vector2i(60, 25));
  textBox3->setFontSize(20);
  textBox3->setValue("4");

  slider3->setCallback(
    [textBox3](float value) {
      textBox3->setValue(std::to_string(static_cast<int>(value*10)));
    });

  slider3->setFinalCallback(
    [&](float value) {
      light_count_ = static_cast<int>(value*10);
      controller_->AcceptGUIParameters(robot_fear_count_, robot_explore_count_,
        light_count_, food_count_, numerator_value_);
    });

  // *************** SLIDER 4 ************************//
  new nanogui::Label(panel, "Number of Foods", "sans-bold");
  nanogui::Slider *slider4 = new nanogui::Slider(panel);
  slider4->setValue(0.4f);
  slider4->setFixedWidth(100);

  nanogui::TextBox *textBox4 = new nanogui::TextBox(panel);
  textBox4->setFixedSize(nanogui::Vector2i(60, 25));
  textBox4->setFontSize(20);
  textBox4->setValue("4");

  slider4->setCallback(
    [textBox4](float value) {
      textBox4->setValue(std::to_string(static_cast<int>(value*10)));
    });

  slider4->setFinalCallback(
    [&](float value) {
      food_count_ = static_cast<int>(value*10);
      controller_->AcceptGUIParameters(robot_fear_count_, robot_explore_count_,
        light_count_, food_count_, numerator_value_);
    });

  // *************** SLIDER 5 ************************//
  new nanogui::Label(panel, "Numerator Value", "sans-bold");
  nanogui::Slider *slider5 = new nanogui::Slider(panel);
  slider5->setValue(0.6f);  // 1200 / 2000 = .6, where 1200 would be
  slider5->setFixedWidth(100);

  nanogui::TextBox *textBox5 = new nanogui::TextBox(panel);
  textBox5->setFixedSize(nanogui::Vector2i(60, 25));
  textBox5->setFontSize(20);
  textBox5->setValue("1200");

  slider5->setCallback(
    [textBox5](float value) {
      textBox5->setValue(std::to_string(static_cast<int>(value*1999 + 1)));
    });

  slider5->setFinalCallback(
    [&](float value) {
      numerator_value_ = static_cast<int>(value*1999 + 1);
      controller_->AcceptGUIParameters(robot_fear_count_, robot_explore_count_,
        light_count_, food_count_, numerator_value_);
    });

  // Lays out all the components with "15" units of inbetween spacing
  panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
      nanogui::Alignment::Middle, 0, 15));

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
