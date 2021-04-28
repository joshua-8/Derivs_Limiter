#ifndef _DERIVS_LIMITER_H_
#define _DERIVS_LIMITER_H_
#include <Arduino.h>
/**
 * @brief  This library can be used to limit the first and second derivative of a variable as it approaches a target value.
 * 
 * https://github.com/joshua-8/Derivs_Limiter
 */
class Derivs_Limiter {
private:
    float position;
    float velocity;
    float accel;
    unsigned long lastTime;
    float velLimit;
    float accelLimit;
    float target;

public:
    /**
     * @brief  constructor for Derivs_Limiter class
     * @param  velL: (float) velocity limit (units per second)
     * @param  accL: (float) acceleration limit (units per second per second)
     * @param  target: (float) target value to make position approach
     * @param  pos: (float) starting position (default: 0)
     * @param  vel: (float) starting velocity (default: 0)
     */
    Derivs_Limiter(float velL, float accL, float targ = 0, float pos = 0, float vel = 0)
    {
        lastTime = 0;
        velLimit = abs(velL);
        accelLimit = abs(accL);
        target = targ;
        position = pos;
        velocity = vel;
    }
    /**
     * @brief  set position and velocity
     * @param  pos: (float) default: 0
     * @param  vel: (float) default: 0
     */
    void setPositionVelocity(float pos = 0, float vel = 0)
    {
        position = pos;
        velocity = vel;
    }
    /**
     * @brief  set position
     * @param  pos: (float) default: 0
     * @retval (boolean) true if position changed
     */
    boolean setPosition(float pos = 0)
    {
        if (pos != position) {
            position = pos;
            return true;
        }
        return false;
    }
    /**
     * @brief  set velocity
     * @param  vel: (float) default: 0
     * @retval (boolean) true if velocity changed
     */
    boolean setVelocity(float vel = 0)
    {
        if (vel != velocity) {
            velocity = vel;
            return true;
        }
        return false;
    }
    /**
     * @brief  set velocity limit
     * @param  velLim: (float) velocity limit (units per second)
     * @retval (boolean) true if limit changed
     */
    boolean setVelLimit(float velLim)
    {
        if (velLim != velLimit) {
            velLimit = abs(velLim);
            return true;
        }
        return false;
    }
    /**
     * @brief  set acceleration limit
     * @param  accelLim: (float) acceleration limit (units per second per second)
     * @retval (boolean) true if limit changed
     */
    boolean setAccelLimit(float accelLim)
    {
        if (accelLim != accelLimit) {
            accelLimit = abs(accelLim);
            return true;
        }
        return false;
    }
    /**
     * @brief  set velocity and acceleration limits
     * @param  velLim: (float) velocity limit
     * @param  accLim: (float) acceleration limit
     */
    void setVelAccelLimits(float velLim, float accLim)
    {
        setVelLimit(velLim);
        setAccelLimit(accLim);
    }
    /**
     * @brief  get the current velocity
     * @retval (float) (units per second)
     */
    float getVelocity()
    {
        return velocity;
    }
    /**
     * @brief  get the current acceleration
     * @retval (float) (units per second per second)
     */
    float getAcceleration()
    {
        return accel;
    }
    /**
     * @brief  get the current position value, but doesn't calculate anything
     * @retval (float)
     */
    float getPosition()
    {
        return position;
    }
    /**
     * @brief  call this as frequently as possible to calculate all the values
     * @retval (float) position
     */
    float calc()
    {
        return _calc();
    }
    /**
     * @brief  call this as frequently as possible to calculate all the values
     * @param  _target: set the target position
     * @retval (float) position
     */
    float calc(float _target)
    {
        target = _target;
        return _calc();
    }

protected:
    /**
     * @brief  this is where the actual code is
     * @retval (float) position
     */
    float _calc()
    {

        float time = (micros() - lastTime) / 1000000.0;
        if (time == 0) { //if it hasn't been a microsecond since last calculated
            return position;
        }
        if (lastTime == 0) {
            time = 0; //in case there's a delay between starting the program and the first calculation avoid jump at start
        }

        if (abs(target - position) <= abs(velocity * time)) { //basically there
            velocity = 0;
            accel = 0;
            position = target;
        } else {
            if (abs(velocity) < velLimit || !((velocity > 0) == (target - position > 0))) { //if slower than max speed or going the wrong way
                accel = constrain((target - position > 0 ? accelLimit : -accelLimit), -velLimit / time, velLimit / time); //  ...accelerate towards target.
            } else if (abs(velocity) > velLimit + accelLimit * time) { // if going too fast
                accel = constrain((velocity < 0 ? accelLimit : -accelLimit), velLimit / time, velLimit / time); //  ..slow down
            } else { // no acceleration needed
                velocity = constrain(velocity, -velLimit, velLimit); //ensure within velLimit
                accel = 0; //coast (at max speed)
            }

            if ((velocity > 0) == (target - position > 0)) { //going towards target
                float stoppingDistance = sq(velocity) / 2 / accelLimit;
                if (abs(target - position) <= stoppingDistance) { //time to start slowing down
                    accel = -sq(velocity) / 2 / (target - position);
                } else if (abs(target - position - velocity * time) <= stoppingDistance) { //on the border of needing to slow donw
                    accel = -sq(velocity) / 2 / (target - position) / 2; //partial decelleration to reduce "spikes" some
                }
            }
        }

        velocity += accel * time;

        position += velocity * time;

        lastTime = micros();

        return position;
    }
};
#endif
