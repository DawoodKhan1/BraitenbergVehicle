/*! \mainpage 3081 Project Iteration 1
 *
 * \section intro_sec Introduction
 *
 * This is the main page of documentation for this iteration 1.
 * 
 * Currently this program simulates a robot within an environment where there are
 * bases to capture by colliding with them as well as obstacles to avoid. The robot 
 * has a number of lives and wins if all bases are captured and loses if all lives are
 * lost.
 *
 * The design of this project is using the MVC architecture. The model is the arena,
 * the controller is controller, and the viewer is the graphics_arena_viewer.
 *
 * The interactions between the MVC's are regulated by the communication enum where it 
 * commands such as kNewGame, kKeyRight, and kWon.
 * 
 * The arena uses entity_factory to make the obstacles as well as vectors to store the entities.
 *
 * The controller accepts commands from the viewer and sends them to the arena again using the
 * communication enum. In the arena's timestepupdate function it updates all the entities as well
 * as update the game status to win/loss. Handles Play/Pause by checking the status of the flag in
 * graphics arena viewer in its timestep update function (controller stops advancing time if either
 * the game is won/lost/paused).
 *
 * The graphics arena viewer has a menu the user can interact with such as New Game and Play/Pause.
 * It also checks on the status of the arena win/loss condition to send a message to the screen 
 * and the status of the robot to display the lives and the invincibility. 
 *
 * The most notable entity in the arena is the robot. The flow to make the robot move is the 
 * viewer gets input from the keyboard, sends it to the controller using a controller method
 * using the communication enum, which the arena accepts the commands and calls the appropriate
 * method of the robot to follow the input key.
 * 
 * Another important detail about robots are that they become invincibile. The process for this is
 * arena records the time at every timestep into the robot so that during a collision it can
 * record the time that the collision starts as well as set the invincibility flag to true.
 * Then in the robot's timestep update it checks for when the invincibility should run out and then
 * sets the flag to false.
 * 
 * Mobile obstacles in terms of the collision handling and timing works similarly to robot with the
 * key differences being after a collision the obstacles reverses instead of becoming invincibile,
 * the time in which it should reverse it gets reset if the obstacle hits another entity during which
 * it is reversing (Ex. invincibility lasts 2s regardless of additional collisions where as reverse
 * timing resets if the obstacle hits something else).
 */
