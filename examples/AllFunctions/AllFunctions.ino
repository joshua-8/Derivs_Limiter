/**
 *  An example for the Derivs_Limiter library
 *  https://github.com/joshua-8/Derivs_Limiter
 * 
 *  This example shows every function that the library has, 
 *  while it should compile, it is not meant to actually be run
 * 
 *  A more proper reference is here: https://joshua-8.github.io/Derivs_Limiter/html/class_derivs___limiter.html
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>
//Velocity and/or acceleration limits can be set as INFINITY in order to have no limit.

//                       Derivs_Limiter(float _velLimit, float _accelLimit, float _target = 0, float _startPos = 0, float _startVel = 0, bool _preventGoingWrongWay = true, bool _preventGoingTooFast = true, float _posLimitLow = -INFINITY, float _posLimitHigh = INFINITY, float _maxStoppingAccel = INFINITY, float* _posPointer = NULL, float* _velPointer = NULL)
Derivs_Limiter limiter = Derivs_Limiter(100, 75, 0, 0, 0, true, true, -INFINITY, INFINITY, INFINITY, NULL, NULL);

void setup()
{
    limiter.setPositionVelocity(0, 0); //pos, vel
    limiter.setPosition(0);
    limiter.setVelocity(0);
    limiter.setVelLimit(INFINITY);
    limiter.setAccelLimit(INFINITY);
    limiter.setVelAccelLimits(1, 1); //vel, accel
    limiter.getVelLimit();
    limiter.getAccelLimit();
    limiter.resetTime();
    limiter.setPreventGoingWrongWay(true);
    limiter.getPreventGoingWrongWay();
    limiter.setPreventGoingTooFast(true);
    limiter.getPreventGoingTooFast();
    limiter.setMaxStoppingAccel(INFINITY);
    limiter.getMaxStoppingAccel();
    limiter.getLowPosLimit();
    limiter.getHighPosLimit();
    limiter.setLowPosLimit(-INFINITY);
    limiter.setHighPosLimit(INFINITY);
    limiter.setPosLimits(-1, 1);
}

void loop()
{
    float target = 10.0;
    limiter.setTarget(target);
    limiter.getTarget();
    float smoothedPosition = limiter.calc(target);
    smoothedPosition = limiter.calc();
    limiter.getPosition();
    limiter.getVelocity();
    limiter.getAcceleration();
    limiter.isPosAtTarget();
    limiter.isPosNotAtTarget();
    limiter.distToTarget();
    limiter.getLastTime();
    limiter.getTimeInterval();
    limiter.getTargetDelta();
    limiter.getLastTarget();
    limiter.getPositionDelta();
    limiter.getLastPosition();
    limiter.getTargetDeltaPerTime();
    float vel;
    limiter.setVelocityPointer(&vel);
    float pos;
    limiter.setPositionPointer(&pos);
    limiter.setPosition(NAN); //has no effect because NAN
    limiter.setVelLimitForTimedMove(180, 10, 100); //dist, time, maxVel
    limiter.setTargetAndVelLimitForTimedMove(180, 10, 100); //target, time, maxVel
    limiter.resetVelLimitToOriginal();
}
