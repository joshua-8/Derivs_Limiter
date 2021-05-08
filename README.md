# Derivs_Limiter

https://github.com/joshua-8/Derivs_Limiter

[![arduino-library-badge](https://www.ardu-badge.com/badge/Derivs_Limiter.svg?)](https://www.ardu-badge.com/Derivs_Limiter)

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

`void setPositionVelocity(double pos = 0, double vel = 0)`

`boolean setPosition(double pos = 0)` (true if position changed)

`boolean setVelocity(double vel = 0)` (true if velocity changed)

`boolean setVelLimit(double velLim)` (true if limit changed)

`boolean setAccelLimit(double accelLim)` (true if limit changed)

`void setVelAccelLimits(double velLim, double accLim)`

`double getPosition()`

`double getVelocity()`

`double getAcceleration()` _for debugging only._

`void resetTime()` If calc() hasn't been run for a while, use this before starting to use it again to protect from large jumps.


## Notes:

Acceleration is not guaranteed to be below the set limit if the target position is changed while position is changing. The priority is to stop in time even if extra acceleration is needed.

During the part of the profile where velocity is decreased, the acceleration is a bit "spiky" as the code switches back and forth across the threshold for whether or not it needs to start decelerating. Acceleration should not be used directly, only position and velocity are really valid. (Also, note that if the acceleration limit is infinity, this value stays zero when position stops)
