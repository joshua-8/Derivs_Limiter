/**
 *  An example for the Derivs_Limiter library
 *  https://github.com/joshua-8/Derivs_Limiter
 * 
 *  This example shows every function that the library has, 
 *  while it should compile it doesn't really do anything.
 * 
 *  A more proper reference is here: https://joshua-8.github.io/Derivs_Limiter/html/class_derivs___limiter.html
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>
//Velocity and/or acceleration limits can be set as INFINITY in order to have no limit.

Derivs_Limiter limiter = Derivs_Limiter(100, 75, 0, 0, 0, true); //Derivs_Limiter(velocityLimit, accelLimit, targetPos=0, initialPos=0, initialVel=0, preventGoingWrongWay=true)

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
}

void loop()
{
    double target = 10.0; //doubles are used as they may give more resolution than floats on some processors
    limiter.setTarget(target);
    limiter.getTarget();
    double smoothedPosition = limiter.calc(target);
    smoothedPosition = limiter.calc();
    limiter.getPosition();
    limiter.getVelocity();
    limiter.getPosDelta();
    limiter.getAcceleration();
    limiter.getTimeInterval();
}