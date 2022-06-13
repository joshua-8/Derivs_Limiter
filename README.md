# Derivs_Limiter

https://github.com/joshua-8/Derivs_Limiter

[![arduino-library-badge](https://www.ardu-badge.com/badge/Derivs_Limiter.svg?)](https://www.ardu-badge.com/Derivs_Limiter)
[![PlatformIO](https://badges.registry.platformio.org/packages/joshua1024/library/Derivs_Limiter.svg)](https://registry.platformio.org/libraries/joshua1024/Derivs_Limiter)

[DETAILED DOCUMENTATION](https://joshua-8.github.io/Derivs_Limiter/class_derivs___limiter.html) (made with Doxygen)

Please ask any questions [here](https://github.com/joshua-8/Derivs_Limiter/discussions).

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


    // all settings of constructor

    limiter.setVelLimit(10);
    limiter.setAccelLimit(5);
    limiter.setDecelLimit(NAN);
    limiter.setTarget(0);
    limiter.setPosition(0);
    limiter.setVelocity(0);
    limiter.setPreventGoingWrongWay(false);
    limiter.setPreventGoingTooFast(false);
    limiter.setLowPosLimit(-INFINITY);
    limiter.setHighPosLimit(INFINITY);
    limiter.setMaxStoppingDecel(2);
    limiter.setPositionPointer(NULL);
    limiter.setVelocityPointer(NULL);

    /// settings grouped together for your convenience

    limiter.setPositionVelocity(0, 0); // position, velocity
    limiter.setTargetAndPosition(0, 0); // target, position
    limiter.setAccelAndDecelLimits(1, NAN); // accel, decel
    limiter.setVelAccelLimits(1, 1, 1); // vel, accel, decel
    limiter.setPosLimits(-INFINITY, INFINITY); // low limit, high limit
    limiter.setPositionAndTarget(0); // both position and target get set to the same value

    // get output

    limiter.getPosition();
    limiter.getVelocity();
    limiter.getAcceleration();

    // get settings

    limiter.getTarget();
    limiter.getVelLimit();
    limiter.getAccelLimit();
    limiter.getDecelLimit();
    limiter.getMaxStoppingDecel();
    limiter.getLowPosLimit();
    limiter.getHighPosLimit();

    // other
    limiter.jogPosition(1); // increments target and position
    limiter.resetTime();
    limiter.getLastTime();
    limiter.getTimeInterval();
    limiter.getTargetDelta();
    limiter.getLastTarget();
    limiter.getPositionDelta();
    limiter.getLastPosition();
    limiter.getTargetDeltaPerTime();
    limiter.getPreventGoingWrongWay();
    limiter.getPreventGoingTooFast();
    limiter.isPosAtTarget();
    limiter.isPosNotAtTarget();
    limiter.distToTarget();

    // velocity mode

    limiter.setVelConstant(1); // sets velocity immediately
    limiter.setVelTarget(1); // velocity approaches the set target with limited accel and decel
    limiter.isPosModeNotVelocity();
    limiter.getVelTarget();

    // timed moves

    limiter.setVelLimitForTimedMove(20, 5, NAN); // dist, time, maxVel
    limiter.setTargetAndVelLimitForTimedMove(10, 5, NAN); // target, time, maxVel
    limiter.setTargetTimedMovePreferred(10, 5, NAN); // target, time, maxVel
    limiter.resetVelLimitToOriginal();


## Notes:

During the part of the profile where velocity is decreased, the acceleration is a bit "spiky" as the code switches back and forth across the threshold for whether or not it needs to start decelerating. Acceleration should not be used directly, only position and velocity are really valid. (Also, note that if the deceleration limit is infinity, this value stays zero when position stops)

NAN (not a number) values are ignored.

[Here](https://gist.github.com/joshua-8/3209f2f400a0e68dead911b8743fc5f0) is a Processing sketch that I used to test the formula I wrote for this library.
