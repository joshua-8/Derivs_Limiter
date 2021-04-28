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

`Derivs_Limiter(velocityLimit, accelLimit, targetPos,initialPos, initialVel)`

### Use:

`smoothedPosition = myLimiter.calc()`

`smoothedPosition = myLimiter.calc(targetPos)`

### Settings and other methods:

`void setPositionVelocity(float pos = 0, float vel = 0)`

`boolean setPosition(float pos = 0)` (true if position changed)

`boolean setVelocity(float vel = 0)` (true if velocity changed)

`boolean setVelLimit(float velLim)` (true if limit changed)

`boolean setAccelLimit(float accelLim)` (true if limit changed)

`void setVelAccelLimits(float velLim, float accLim)`

`float getVelocity()`

`float getAcceleration()`

`float getPosition()`

## Notes:

During the part of the profile where velocity is decreased, the acceleration is a bit "spiky" as the code switches back and forth across the threshold for whether or not it needs to start decelerating.


[This sketch](https://gist.github.com/joshua-8/3209f2f400a0e68dead911b8743fc5f0) made with [Processing](https://processing.org/) uses the same formula as I made for this library.
