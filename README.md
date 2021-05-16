# Derivs_Limiter

https://github.com/joshua-8/Derivs_Limiter

[![arduino-library-badge](https://www.ardu-badge.com/badge/Derivs_Limiter.svg?)](https://www.ardu-badge.com/Derivs_Limiter) Also available for [PlatformIO](https://platformio.org/lib/show/12113/Derivs_Limiter)

[DETAILED DOCUMENTATION](https://joshua-8.github.io/Derivs_Limiter/html/class_derivs___limiter.html) (made with Doxygen)

This library can be used to limit the first and second derivative of a variable as it approaches a target value.

It's easiest to think of in terms of position, velocity, and acceleration. 
If used with a servo, for example, the servo will smoothly move to a target value with a trapezoidal velocity profile.

The formula in this program supports being run at uneven intervals, and allows for editing the target, position, and velocity while it runs since it doesn't rely on calculating a motion profile ahead of time.

## Usage:

### Create a Derivs_Limiter object:
`Derivs_Limiter myLimiter = Derivs_Limiter(velocityLimit, accelLimit);`

`Derivs_Limiter(velocityLimit, accelLimit, targetPos, initialPos, initialVel)`

_**Velocity and/or acceleration limits can be set as INFINITY in order to have no limit.**_

### Use:

`smoothedPosition = myLimiter.calc()`

`smoothedPosition = myLimiter.calc(targetPos)`

### Settings and other methods:

`bool setTarget(double _target)` (true if target reached)

`double getTarget()`

`void setPositionVelocity(double pos = 0, double vel = 0)`

`bool setPosition(double pos = 0)` (true if position changed)

`bool setVelocity(double vel = 0)` (true if velocity changed)

`bool setVelLimit(double velLim)` (true if limit changed)

`bool setAccelLimit(double accelLim)` (true if limit changed)

`double getVelLimit()`

`double getAccelLimit()`

`void setVelAccelLimits(double velLim, double accLim)`

`double getPosition()`

`double getVelocity()`

`double getAcceleration()` _for debugging only._

`void resetTime()` If calc() hasn't been run for a while, use this before starting to use it again to protect from large jumps.

`double getTimeInterval()`

`double getPosDelta()` (how much did position just change?)

`setPreventGoingWrongWay(bool preventGoingWrongWay)` true = immediately set velocity to zero if moving away from target, false = stay under accel limit

`bool getPreventGoingWrongWay()`

## Notes:

Acceleration is not guaranteed to be below the set limit if the target position is changed while position is changing. The priority is to stop in time even if extra acceleration is needed.

During the part of the profile where velocity is decreased, the acceleration is a bit "spiky" as the code switches back and forth across the threshold for whether or not it needs to start decelerating. Acceleration should not be used directly, only position and velocity are really valid. (Also, note that if the acceleration limit is infinity, this value stays zero when position stops)

The type `double` is the same as a float on standard Arduinos, but can provide increased precision on some more powerful processors. 
