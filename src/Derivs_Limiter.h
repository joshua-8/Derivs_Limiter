#ifndef _DERIVS_LIMITER_H_
#define _DERIVS_LIMITER_H_
#include <Arduino.h>
/**
 * @brief  This library can be used to limit the first and second derivative of a variable as it approaches a target value.
 * https://github.com/joshua-8/Derivs_Limiter
 */
class Derivs_Limiter {
private:
    double position;
    double velocity;
    double accel;
    unsigned long lastTime;
    double velLimit;
    double accelLimit;
    double target;
    double lastPosition;
    double time;

public:
    /**
     * @brief  constructor for Derivs_Limiter class
     * @param  velL: (double) velocity limit (units per second)
     * @param  accL: (double) acceleration limit (units per second per second)
     * @param  target: (double) target value to make position approach (default: 0)
     * @param  pos: (double) starting position (default: 0)
     * @param  vel: (double) starting velocity (default: 0)
     */
    Derivs_Limiter(double velL, double accL, double targ = 0, double pos = 0, double vel = 0)
    {
        lastTime = 0;
        velLimit = abs(velL);
        accelLimit = abs(accL);
        target = targ;
        position = pos;
        velocity = vel;
        lastPosition = pos;
        time = 0;
    }
    /**
     * @brief  set position and velocity
     * @param  pos: (double) default: 0
     * @param  vel: (double) default: 0
     */
    void setPositionVelocity(double pos = 0, double vel = 0)
    {
        position = pos;
        velocity = vel;
    }
    /**
     * @brief  set position
     * @param  pos: (double) default: 0
     * @retval (boolean) true if position changed
     */
    boolean setPosition(double pos = 0)
    {
        if (pos != position) {
            position = pos;
            return true;
        }
        return false;
    }
    /**
     * @brief  set velocity
     * @param  vel: (double) default: 0
     * @retval (boolean) true if velocity changed
     */
    boolean setVelocity(double vel = 0)
    {
        if (vel != velocity) {
            velocity = vel;
            return true;
        }
        return false;
    }
    /**
     * @brief  set velocity limit
     * @param  velLim: (double) velocity limit (units per second)
     * @retval (boolean) true if limit changed
     */
    boolean setVelLimit(double velLim)
    {
        if (velLim != velLimit) {
            velLimit = abs(velLim);
            return true;
        }
        return false;
    }
    /**
     * @brief  set acceleration limit
     * @param  accelLim: (double) acceleration limit (units per second per second)
     * @retval (boolean) true if limit changed
     */
    boolean setAccelLimit(double accelLim)
    {
        if (accelLim != accelLimit) {
            accelLimit = abs(accelLim);
            return true;
        }
        return false;
    }
    /**
     * @brief  set velocity and acceleration limits
     * @param  velLim: (double) velocity limit
     * @param  accLim: (double) acceleration limit
     */
    void setVelAccelLimits(double velLim, double accLim)
    {
        setVelLimit(velLim);
        setAccelLimit(accLim);
    }
    /**
     * @brief  get the current velocity
     * @retval (double) (units per second)
     */
    double getVelocity()
    {
        return velocity;
    }
    /**
     * @brief  get the current acceleration
     * @note for debugging only, value noisy
     * @retval (double) (units per second per second)
     */
    double getAcceleration()
    {
        return accel;
    }
    /**
     * @brief  get the current position value, but doesn't calculate anything
     * @retval (double)
     */
    double getPosition()
    {
        return position;
    }
    /**
     * @brief  call this as frequently as possible to calculate all the values
     * @retval (double) position
     */
    double calc()
    {
        return _calc();
    }
    /**
     * @brief  call this as frequently as possible to calculate all the values
     * @param  _target: set the target position
     * @retval (double) position
     */
    double calc(double _target)
    {
        target = _target;
        return _calc();
    }
    /**
     * @brief  set target position (doesn't run calculation, make sure to run calc() yourself)
     * @param  _target: (double) position
     * @retval  (boolean) position==target
     */
    boolean setTarget(double _target)
    {
        target = _target;
        return position == target;
    }
    /**
     * @brief If calc hasn't been run for a while, use this before starting to use it again to protect from large jumps.
     */
    void resetTime()
    {
        lastTime = micros();
    }

    /**
     * @brief  returns the time (in seconds) between the two most recent calculation times
     * @retval  (double)
     */
    double getTimeInterval()
    {
        return time;
    }

    /**
     * @brief  returns the change in position from the most recent run of calc()
     * @retval  (double)
     */
    double getPosDelta()
    {
        return position - lastPosition;
    }

private:
    /**
     * @brief  this is where the actual code is
     * @retval (double) position
     */
    double _calc()
    {
        lastPosition = position;
        time = (micros() - lastTime) / 1000000.0;
        if (time == 0) { //if it hasn't been a microsecond since last calculated
            return position;
        }

        if (lastTime == 0) {
            //in case there's a delay between starting the program and the first calculation avoid jump at start by just waiting for next time
            lastTime = micros();
            return position;
        }

        lastTime = micros();

        if (abs(target - position) <= abs(velocity * time)) { //basically there, set position to target
            velocity = 0;
            accel = 0;
            position = target;

        } else {
            if (abs(velocity) < velLimit || !((velocity > 0) == (target - position > 0))) { //if slower than max speed or going the wrong way
                accel = constrain((target - position > 0 ? accelLimit : -accelLimit), -velLimit / time, velLimit / time); //...accelerate towards target.
            } else if (abs(velocity) > velLimit + accelLimit * time) { //if going too fast
                accel = constrain((velocity < 0 ? accelLimit : -accelLimit), -velLimit / time, velLimit / time); //...slow down
            } else { //no acceleration needed
                velocity = constrain(velocity, -velLimit, velLimit); //ensure within velLimit
                accel = 0; //coast (at max speed)
            }

            if ((velocity > 0) == (target - position > 0)) { //going towards target
                double stoppingDistance = sq(velocity) / 2 / accelLimit;
                if (abs(target - position) <= stoppingDistance) { //time to start slowing down
                    accel = -sq(velocity) / 2 / (target - position);
                } else if (abs(target - position - velocity * time) <= stoppingDistance) { //on the border of needing to slow donw
                    accel = -sq(velocity) / 2 / (target - position) / 2; //partial decelleration to reduce "spikes" some
                }
            }
        }

        if (abs(target - position) <= abs(velocity * time + accel * time)) { //basically there, set position to target (recheck)
            velocity = (target - position) / time;
            accel = 0;
            position = target;

        } else {
            velocity += accel * time;
            position += velocity * time;
        }

        return position;
    }
};
#endif
