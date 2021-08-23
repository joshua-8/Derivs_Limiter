# Derivs_Limiter

https://github.com/joshua-8/Derivs_Limiter

[![arduino-library-badge](https://www.ardu-badge.com/badge/Derivs_Limiter.svg?)](https://www.ardu-badge.com/Derivs_Limiter) Also available for [PlatformIO](https://platformio.org/lib/show/12113/Derivs_Limiter)

[DETAILED DOCUMENTATION](https://joshua-8.github.io/Derivs_Limiter/html/class_derivs___limiter.html) (made with Doxygen)

This library can be used to limit the first and second derivative of a variable as it approaches a target value.

It's easiest to think of in terms of position, velocity, and acceleration. 
If used with a servo, for example, the servo will smoothly move to a target value with a trapezoidal velocity profile.

You can also specify how long you want a move to take and the library can calculate the velocity needed to make that happen.

The formula in this program supports being run at uneven intervals, and allows for editing the target, position, and velocity while it runs since it doesn't rely on calculating a motion profile ahead of time.

## Usage:

### Create a Derivs_Limiter object:
`Derivs_Limiter myLimiter = Derivs_Limiter(velocityLimit, accelLimit);`

`Derivs_Limiter(float _velLimit, float _accelLimit, float _target = 0, float _startPos = 0, float _startVel = 0, bool _preventGoingWrongWay = true, bool _preventGoingTooFast = true, float _posLimitLow = -INFINITY, float _posLimitHigh = INFINITY, float _maxStoppingAccel = INFINITY, float* _posPointer = NULL, float* _velPointer = NULL)`

_**Velocity, position and/or acceleration limits can be set as INFINITY in order to have no limit.**_

### quick reference: ([more detailed reference available here](https://joshua-8.github.io/Derivs_Limiter/html/class_derivs___limiter.html))

  `  float calc()`  

  `  float calc(float _target)`  

  `  bool setTarget(float _target)`  

  `  float getTarget()`  

  `  float getPosition()`  

  `  float getVelocity()`  

  `  float getAcceleration()`

  `  void setPositionVelocity(float pos = 0, float vel = 0)`  

  `  bool setPosition(float pos = 0)`  

  `  bool setVelocity(float vel = 0)`  

  `  bool setVelLimit(float velLim)`  

  `  bool setAccelLimit(float accelLim)`  

  `  float getVelLimit()`  

  `  float getAccelLimit()`  

  `  void setVelAccelLimits(float velLim, float accLim)`    

  `  void setMaxStoppingAccel(float _maxStoppingAccel = INFINITY)`  

  `  float getMaxStoppingAccel()`  

  `  float getLowPosLimit()`  

  `  float getHighPosLimit()`  

  `  bool setLowPosLimit(float lowLimit)`  

  `  bool setHighPosLimit(float highLimit)`  

  `  void setPosLimits(float lowLimit, float highLimit)`  

  `  void resetTime()`  

  `  unsigned long getLastTime()`  

  `  float getTimeInterval()`  

  `  float getTargetDelta()`  

  `  float getLastTarget()`  
  
  `  float getPositionDelta()`

  `  float getLastPosition()`

  `  float getTargetDeltaPerTime()`  

  `  void setPositionPointer(float* _positionPointer)`  

  `  void setVelocityPointer(float* _velocityPointer)`  

  `  bool getPreventGoingWrongWay()`  

  `  void setPreventGoingWrongWay(bool _preventGoingWrongWay)`  

  `  bool getPreventGoingTooFast()`  

  `  void setPreventGoingTooFast(bool _preventGoingTooFast)`  

  `  bool isPosAtTarget()`  

  `  bool isPosNotAtTarget()`  

  `  float distToTarget()`  

  `  void resetVelLimitToOriginal()`

  `  boolean setVelLimitForTimedMove(float _dist, float _time, float _maxVel = NAN)`

  `  boolean setTargetAndVelLimitForTimedMove(float _target, float _time, float _maxVel = NAN)`


## Notes:

During the part of the profile where velocity is decreased, the acceleration is a bit "spiky" as the code switches back and forth across the threshold for whether or not it needs to start decelerating. Acceleration should not be used directly, only position and velocity are really valid. (Also, note that if the acceleration limit is infinity, this value stays zero when position stops)

NAN (not a number) values are ignored.

[Here](https://gist.github.com/joshua-8/3209f2f400a0e68dead911b8743fc5f0) is a Processing sketch that I used to test the formula I wrote for this library.
