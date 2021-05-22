#ifndef _DERIVS_LIMITER_H_
#define _DERIVS_LIMITER_H_
#include <Arduino.h>
/**
 * @brief  This library can be used to limit the first and second derivative of a variable as it approaches a target value.
 * https://github.com/joshua-8/Derivs_Limiter
 */
class Derivs_Limiter {
protected:
    float position;
    float velocity;
    float accel;
    unsigned long lastTime;
    float target;
    float velLimit;
    float accelLimit;
    float time;
    bool preventGoingWrongWay;
    bool preventGoingTooFast;
    float posLimitLow;
    float posLimitHigh;
    float maxStoppingAccel;
    float lastTarget;
    float* positionPointer;
    float* velocityPointer;

public:
    Derivs_Limiter(float _velLimit, float _accelLimit, float _target = 0,
        float _startPos = 0, float _startVel = 0, bool _preventGoingWrongWay = true, bool _preventGoingTooFast = true,
        float _posLimitLow = -INFINITY, float _posLimitHigh = INFINITY, float _maxStoppingAccel = 10.0,
        float* _posPointer = NULL, float* _velPointer = NULL)
    {
        accel = 0;
        lastTime = 0;
        velLimit = abs(_velLimit);
        accelLimit = abs(_accelLimit);
        target = _target;
        lastTarget = _target;
        position = _startPos;
        velocity = _startVel;
        time = 0;
        preventGoingWrongWay = _preventGoingWrongWay;
        preventGoingTooFast = _preventGoingTooFast;
        posLimitLow = _posLimitLow;
        posLimitHigh = max(_posLimitHigh, posLimitLow);
        maxStoppingAccel = max(_maxStoppingAccel, 1.0);
        positionPointer = _posPointer;
        velocityPointer = _velPointer;
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
     * @retval (bool) true if position changed
     */
    bool setPosition(float pos = 0)
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
     * @retval (bool) true if velocity changed
     */
    bool setVelocity(float vel = 0)
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
     * @retval (bool) true if limit changed
     */
    bool setVelLimit(float velLim)
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
     * @retval (bool) true if limit changed
     */
    bool setAccelLimit(float accelLim)
    {
        if (accelLim != accelLimit) {
            accelLimit = abs(accelLim);
            return true;
        }
        return false;
    }

    /**
     * @brief  get velocity limit setting
     * @retval  (float)
     */
    float getVelLimit()
    {
        return velLimit;
    }

    /**
     * @brief  get acceleration limit setting
     * @retval  (float)
     */
    float getAccelLimit()
    {
        return accelLimit;
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
     * @note for debugging only, value noisy
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

    /**
     * @brief  set target position (doesn't run calculation, make sure to run calc() yourself)
     * @param  _target: (float) position
     * @retval  (bool) position==target
     */
    bool setTarget(float _target)
    {
        target = _target;
        return position == target;
    }

    /**
     * @brief  get target position
     * @retval  (float)
     */
    float getTarget()
    {
        return target;
    }

    /**
     * @brief If calc hasn't been run for a while, use this before starting to use it again to protect from large jumps.
     */
    void resetTime()
    {
        lastTime = micros();
    }

    /**
     * @brief  returns the value of micros() when calc() last ran
     * @retval  unsigned long
     */
    unsigned long getLastTime()
    {
        return lastTime;
    }

    /**
     * @brief  returns the time (in seconds) between the two most recent calculation times
     * @retval  (float)
     */
    float getTimeInterval()
    {
        return time;
    }

    /**
     * @brief  returns the change in target from the most recent run of calc()
     * @retval  (float)
     */
    float getTargetDelta()
    {
        return target - lastTarget;
    }

    /**
     * @brief  returns value of preventGoingWrongWay setting
     * @retval (bool)
     */
    bool getPreventGoingWrongWay()
    {
        return preventGoingWrongWay;
    }

    /**
     * @brief  sets value of preventGoingWrongWay, true = immediately set velocity to zero if moving away from target, false = stay under accel limit
     * @param  _preventGoingWrongWay: (bool)
     */
    void setPreventGoingWrongWay(bool _preventGoingWrongWay)
    {
        preventGoingWrongWay = _preventGoingWrongWay;
    }

    /**
     * @brief  returns value of preventGoingTooFast setting
     * @retval (bool)
     */
    bool getPreventGoingTooFast()
    {
        return preventGoingTooFast;
    }

    /**
     * @brief  sets value of preventGoingWrongWay, true = immediately set velocity to zero if moving away from target, false = stay under accel limit
     * @param  _preventGoingTooFast: (bool)
     */
    void setPreventGoingTooFast(bool _preventGoingTooFast)
    {
        preventGoingTooFast = _preventGoingTooFast;
    }

    /**
     * @brief  does position equal target?
     * @retval  (bool)
     */
    bool isPosAtTarget()
    {
        return position == target;
    }

    /**
     * @brief  is position not equal to target?
     * @retval  (bool)
     */
    bool isPosNotAtTarget()
    {
        return position != target;
    }

    /**
     * @brief  returns position - target
     * @note use abs(distToTarget()) if you don't care about direction
     * @retval (float)
     */
    float distToTarget()
    {
        return position - target;
    }

protected:
    /**
     * @brief  this is where the actual code is
     * @retval (float) position
     */

    float _calc()
    {
        if (positionPointer)
            position = *positionPointer;

        if (velocityPointer)
            velocity = *velocityPointer;

        time = (micros() - lastTime) / 1000000.0;
        if (lastTime == 0) {
            time = 0; //in case there's a delay between starting the program and the first calculation avoid jump at start
            lastTime = micros();
        }
        if (time == 0) {
            return position;
        }
        lastTime = micros();

        if (position > posLimitHigh) {
            position = posLimitHigh;
            velocity = 0;
        } else if (position < posLimitLow) {
            position = posLimitLow;
            velocity = 0;
        }
        target = constrain(target, posLimitLow, posLimitHigh);
        lastTarget = target;
        if (preventGoingTooFast)
            velocity = constrain(velocity, -velLimit, velLimit);

        if ((target == position && velocity == 0) || (abs(target - position) <= abs(velocity * time) && (abs(velocity) < accelLimit * time * maxStoppingAccel))) { //basically there
            accel = 0;
            velocity = 0;
            position = target;
        } else { //need to move
            float maxAccelA = min(accelLimit, velLimit / time);
            accel = (((target - position) > 0 ? velLimit : -velLimit) - velocity) / time;
            accel = constrain(accel, -maxAccelA, maxAccelA);
            if (velocity == 0 || (velocity > 0) == (target - position > 0)) { //going towards target
                float maxAccelB = abs((target - position) / time / time);
                accel = constrain(accel, -maxAccelB, maxAccelB);
            }

            if (preventGoingWrongWay && velocity != 0 && (velocity > 0) != (target - position > 0)) //going the wrong way
                velocity = 0;

            if (velocity != 0 && (velocity > 0) == (target - position > 0)) { //going towards target
                float stoppingDistance = sq(velocity) / 2 / accelLimit;
                if (abs(target - position) <= stoppingDistance) { //time to start slowing down
                    accel = -sq(velocity) / 2 / (target - position);
                    accel = constrain(accel, -accelLimit * maxStoppingAccel, accelLimit * maxStoppingAccel);
                } else if (abs(target - position - velocity * time) <= stoppingDistance) { //on the border of needing to slow down
                    accel = -sq(velocity) / 2 / (target - position) / 2; //partial decelleration to reduce "spikes" some
                    accel = constrain(accel, -accelLimit * maxStoppingAccel, accelLimit * maxStoppingAccel);
                }
            }
            velocity += accel * time;
            position += velocity * time;
        }
        if (positionPointer)
            *positionPointer = position;
        if (velocityPointer)
            *velocityPointer = velocity;

        return position;
    }
};
#endif
