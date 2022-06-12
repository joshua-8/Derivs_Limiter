/**
 *  An example for the Derivs_Limiter library
 *  https://github.com/joshua-8/Derivs_Limiter
 *
 *  This example shows every function that the library has,
 *  while it should compile, it is not meant to actually be run
 *
 *  A more proper reference is here: https://joshua-8.github.io/Derivs_Limiter/class_derivs___limiter.html
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>
// Velocity and/or acceleration limits can be set as INFINITY in order to have no limit.

//    Derivs_Limiter(float _velLimit, float _accelLimit, float _decelLimit = NAN, float _target = 0, float _startPos = 0, float _startVel = 0, bool _preventGoingWrongWay = false, bool _preventGoingTooFast = false, float _posLimitLow = -INFINITY, float _posLimitHigh = INFINITY, float _maxStoppingDecel = 2, float* _posPointer = NULL, float* _velPointer = NULL)

Derivs_Limiter limiter = Derivs_Limiter(10, 5, NAN, 0, 0, 0, false, false, -INFINITY, INFINITY, 2, NULL, NULL);

void setup()
{

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

    /// settings grouped for your convenience

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
}

void loop()
{

    limiter.calc();

    limiter.calc(10); // calc but first set target

    // calc() should be run as frequently and regularly as possible, though it doesn't need to be run on a strict timer
}
