/**
 * @file mainpage.h
 *
 * @copyright 2018 Dawood Khan
 */

#ifndef SRC_MAINPAGE_H_
#define SRC_MAINPAGE_H_

/*! \mainpage Iteration 3 Documentation
 *
 * \section user_intro_sec User Guide
 *
 * When initially opening the application you will be presented with a menu
 * on the left hand side and there will be a grid with robots, lights, food,
 * and walls. 
 *
 * \section robot_info_sec Robot Information
 *
 * For robots there are 4 different behaviors: explore, fear, love, and
 * aggressive. Explore robots move fastest away from any lights and move
 * slowest when close to lights but still avoid colliding with them. Fear
 * robots move fastest near lights and slowest far away from any lights but
 * they avoid collisions with lights. Love robots move slowly towards lights
 * and speed up the closer they are to one but goes straight towards them.
 * Aggressive robots move fastest away from lights and move slower closer
 * during lights but goes straight towards them. Currently, the application
 * does not have any robots with love behavior.
 *
 * Robots have a special type of motions controlled by two wheels
 * (robot representation will be explained later). The robot does not move
 * in a simple up, down, left, and right motion. The motion the robot takes
 * depends on the difference in the two wheel velocities. The wheel
 * velocities can only have positive or zero value as well as having a
 * maximum velocity possible.
 *
 * Robots are controlled via their sensor readings. Robots have a left and
 * right sensor and based on these sensors the robots velocity is affected.
 * These sensors are represented by the two white circles on the robot,
 *
 * \section other_entities_sec Other Entities Information
 *
 * There are also lights which are white that move in a certain direction at
 * constant speed. Foods do not move and are red. Walls are the 4 white
 * lines forming a rectangle.
 *
 * There are also lights which are white that move in a certain direction at
 * constant speed. Foods do not move and are red. Walls are the 4 white lines
 *  forming a rectangle.
 *
 * \section collision_sec Collisions
 *
 * The way collisions are handled is that there are rules governing what type
 *  of object collides with what other objects. Robots collide with other
 * robots and the walls. Lights collide with other lights and walls. Nothing
 * collides with food. When a mobile object collides with another object,
 * the object reverses in an arc for a set period of time to avoid further
 * collisions (mobile objects being robots and lights).
 *
 * \section simulation_purpose_sec Simulation Purpose
 *
 * With all the prerequisite information out of the way we can address the
 * main point of the simulation/application. The application simulates robots
 * moving around in a limited environment with objects to avoid (lights),
 * objects to gravitate to (food), and changing behavior depending on the
 * situation (robot hunger).
 *
 * \section advanced_robot_info_sec Advanced Robot Information
 *
 * The robot has a few states. The states are not hungry, hungry, starving,
 * and dead. These states are represented graphically by the robot being
 * green, yellow, red, and purple respectively. It takes about 30 seconds
 * for a robot to become hungry, 2 minutes to become starving, and
 * 2.5 minutes to become dead. When any robot becomes dead, the simulation
 * ends and “Simulation Over - Robot has Died!” will be displayed on the
 * screen. A robot can only reset its hunger state is for it to become 5
 * pixels of a food object.
 *
 * \section gui_sec GUI (Graphical User Interface)
 *
 * Now we can take a look at the menu with the graphical user interface (GUI).
 * The GUI is divided into two parts, the Simulation Control and Arena
 * Configuration.
 *
 * The simulation controls controls has two buttons a Play/Pause button and a
 * New Game button. The Play/Pause button of course resumes or pauses the
 * simulation. If the button says Play, that resumes the simulation and if the
 * button says Pause then it pauses the simulation. Initially the simulation is
 * paused so the user must click Play to start the simulation. The New Game
 * button resets all entities to their default states and places them randomly
 * in the arena.
 *
 * The arena configuration section allows the user to control various elements
 * using a slider. These values are the number of robots of a certain type,
 * number of specific entities, whether hunger is included in simulation or
 * not, and how sensitive robots are to the lights. These changes are changed
 * instantly once the user lets go of the slider. The number of fear and
 * explore robots are allowed to be controlled and can go from 0-5 robots.
 * The number of lights and foods can go from 0-10. Hunger is disabled when
 * the number of foods is 0, when this happens robots do not feel hunger and
 * therefore cannot starve to death. The numerator value controls how
 * sensitive the robots are to lights. It goes from 1-2000. The lower the
 * number the less sensitive the robots are to lights and vice versa going
 * to higher numbers.
 *
 * \section dev_guide_sec Dev Guide
 *
 * \section dev_intro_sec Introduction
 *
 * Before reading this section read the User Guide. Information presented
 * there will not be repeated here and it will be assumed you know
 * everything covered in the User Guide.
 *
 * Before running the simulation the MinGfx library needs to be installed in
 * your computer. The path to the library will be specified in the makefile.
 * MinGfx library uses the openGL graphics library.
 *
 * \section mvc_sec MVC Background
 *
 * This project follows the MVC architecture. The model is the Arena, viewer
 * is the GraphicsArenaViewer, and the controller is Controller.
 *
 * The Arena is the model in the MVC. It holds a pointer to all the entities
 * in the arena. It has a vector of pointers to all entities, mobile entities,
 * foods, robots, and lights. These vectors are how the arena starts the
 * interaction between entities. For example, it goes through the robot
 * vector to send the position information of lights and foods to the
 * robot’s sensors.  The function where this is done is TimestepUpdate which
 * is a critical method. In this function Arena advances time as well as
 * notifies entities of anything they need to be made aware of: collisions,
 * position of light objects, etc. This function also checks loss condition
 * which is if any robot has starved. Arena also has a pointer to
 * EntityFactory which is what creates all the entities in the arena:
 * robots, lights, and foods.
 *
 * The GraphicsArenaViewer is what displays the information from the arena to
 * the window. The function to do this display is DrawUsingNanoVG. This
 * function calls a method to draw the arena entities and a separate function
 * to draw the robot. In this DrawRobot function it draws the robot with
 * different colors depending on its hunger level as well as its light sensors
 * (food sensors are not drawn). GraphicsArenaViewer also checks on the game
 * status from its arena pointer and if a robot has died it displays a loss
 * message. This also manages the GUI which includes the Play/Pause button,
 * New Game button, and the sliders to control various elements
 * of the simulation.
 *
 * The Controller is what governs interactions between the Arena and the
 * GraphicsArenaViewer. This means any communication between the
 * GraphicsArenaViewer and the Arena must go through the Controller.
 * For example, if a slider value changes in the GUI, the GraphicsArenaViewer
 * has a function that passes the changes in values to the Controller, which
 * passes the information to the appropriate function on Arena.
 *
 * \section braitenberg_vehicle_sec Braitenberg Vehicle Implementation
 *
 * The sensor motor connections are different for each of the four behavior.
 * Here there will be the definitions of a couple of terms before we go into
 * the exact nature of the connections. Positive means robots get faster the
 * closer they get to the light (negative means opposite), and direct means
 * the left sensor affects the velocity of the left wheel
 * (crossed is opposite wiring). Explore has negative crossed connection,
 * fear has positive direct connection, love has negative direct connection,
 * and agressive has a positive crossed connection.
 *
 * \section strategy_pattern_sec Strategy Pattern
 *
 * This is implemented with a Strategy Pattern. In this design the interface
 * is RobotBehavior. This is the parent class which has 4 subclasses:
 * AgressiveBehavior, FearBehavior, LoveBehavior, and ExploreBehavior.
 *
 * The parent class has a virtual method called processReading which takes
 * in 2 parameters which are the readings of the left sensor and right sensor
 * respectively. It then returns a WheelVelocity after running an algorithm to
 * determine how to convert the sensor readings into a WheelVelocity object.
 * It also has a protected member called light_max_reading_ for the algorithms
 * used in the child classes LoveBehavior and ExploreBehavior.
 *
 * In robot.h there is a RobotBehavior * robot_behavior_ member that is
 * initialized to a new instance of one of the 4 child classes in arena’s
 * constructor depending on the type of the robot through a setter.
 *
 * In robot’s TimestepUpdate function, the WheelVelocity object is made by
 * calling robot_behavior_->processReading(), passing in the left and right
 * light sensor readings in that order for the light sensor readings.
 * For the food sensor readings there is a
 * RobotBehavior * food_behavior_ member that is set by default to
 * new AgressiveBehavior() because robot’s will always be aggressive
 * towards food if they are hungry.
 *
 * \section observer_pattern_sec Observer Pattern
 *
 * The observer pattern implemented is where the the subjects are the food,
 * light, and robot (all entities) and will be using the position information
 * would get_pose() method in ArenaEntity which all these entities inherit
 * from. The arena would be the observer instead of the various sensors.
 * The arena would get the position information for all the entities in the
 * arena and then pass that information along to all the sensors. The sensors
 * would then do all the calculations it has to and either moves the robot
 * within the sensor or gives the processed data back to robot so the robot
 * could do what it needs to.
 *
 * The robot would again based on the sensor motor connection move
 * appropriately the wheel velocities after processing the data from
 * the sensors. The way this happens is robot’s TimestepUpdate function first
 * it processes the reading from the sensors with the
 * robot_behavior_ data member and then calls MotionHandler’s UpdateVelocity
 * with the WheelVelocity object that is returned from processReading from
 * RobotBehavior’s child classes. Of course it also has flag variables
 * depending on the hunger levels what readings should be taken into account.
 */

#endif  // SRC_MAINPAGE_H_
