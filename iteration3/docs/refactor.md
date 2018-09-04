Replace Magic Number with Symbolic Constant

Original Code:
food_sensor.cc (line 33):  / (pow(calculateDistance(position), 1.08)));

light_sensor.cc (line 32):  (pow(calculateDistance(position), 1.08)));

Want to replace the 1.08 in the equation to a class variable.

Code Changes:
Light_sensor.h: (addition)
 private:
  double base_{1.08};

food_sensor.cc (line 33):  / (pow(calculateDistance(position), base_)));


Food_sensor.h: (addition)
 private:
  double base_{1.08};

light_sensor.cc (line 32):  (pow(calculateDistance(position), base_)));

Also fixed compiler issue: with int being nt in arena.h

This change is beneficial because if the base value would change from 1.08 for either food sensor or
light sensor, it would not be clear what this number represents and it is not good for code
readability. Making a double base_ private variable in both light_sensor.h and food_sensor.h allows
for code readability because it shows that some constant base is being raised to a power rather than
a random number. This also allows for different base values for light and food sensors.

