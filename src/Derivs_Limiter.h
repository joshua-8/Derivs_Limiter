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
    float decelLimit;
    float time;
    bool preventGoingWrongWay;
    bool preventGoingTooFast;
    float posLimitLow;
    float posLimitHigh;
    float maxStoppingDecel;
    float lastTarget;
    float targetDelta;
    float lastPos;
    float posDelta;
    float originalVelLimit;
    float* positionPointer;
    float* velocityPointer;
    bool posMode;
    float velocityTarget;

public:
    /**
     * @brief  constructor for Derivs_Limiter class
     * @param  _velLimit: (float) velocity limit (units per second)
     * @param  _accelLimit: (float) acceleration limit (units per second per second)
     * @param  _decelLimit: (float) default=NAN, deceleration limit (units per second per second), set to NAN to use accelLimit
     * @param  _target: (float) default=0, target value to make position approach
     * @param  _startPos: (float) default=0, starting position
     * @param  _startVel: (float) default=0, starting velocity
     * @param  _preventGoingWrongWay: (bool) default=false, stop immediately if velocity is going away from target
     * @param  _preventGoingTooFast: (bool) default=false, constrain velocity to within velLimit
     * @param  _posLimitLow: (float) default=-INFINITY, lower bound for position
     * @param  _posLimitHigh: (float) default=INFINITY, upper bound for position, will be set to _posLimitLow if below _posLimitLow
     * @param  _maxStoppingDecel: (float) default=2, how many times accelLimit can be used to stop in time for target position (can be 1 through INFINITY)
     * @param  _posPointer: set pointer to an external variable that will be read and modified during calc as position.  use &var
     * @param  _velPointer: set pointer to an external variable that will be read and modified during calc as velocity.  use &var
     */
    Derivs_Limiter(float _velLimit, float _accelLimit, float _decelLimit = NAN, float _target = 0,
        float _startPos = 0, float _startVel = 0, bool _preventGoingWrongWay = false, bool _preventGoingTooFast = false,
        float _posLimitLow = -INFINITY, float _posLimitHigh = INFINITY, float _maxStoppingDecel = 2,
        float* _posPointer = NULL, float* _velPointer = NULL)
    {
        accel = 0;
        lastTime = 0;
        velLimit = abs(_velLimit);
        originalVelLimit = velLimit;
        accelLimit = abs(_accelLimit);
        setDecelLimit(_decelLimit);
        target = 0;
        if (!isnan(_target))
            target = _target;
        posMode = true;
        lastTarget = _target;
        targetDelta = 0;
        position = 0;
        if (!isnan(_startPos))
            position = _startPos;
        lastPos = position;
        posDelta = 0;
        velocity = 0;
        if (!isnan(_startVel))
            velocity = _startVel;
        time = 0;
        preventGoingWrongWay = _preventGoingWrongWay;
        preventGoingTooFast = _preventGoingTooFast;
        posLimitLow = _posLimitLow;
        posLimitHigh = max(_posLimitHigh, posLimitLow);
        maxStoppingDecel = max(_maxStoppingDecel, (float)1.0);
        positionPointer = _posPointer;
        velocityPointer = _velPointer;
        velocityTarget = 0;
    }

    /**
     * @brief  default constructor for Derivs_Limiter
     * @note  make sure to use the normal constructor after this, this constructor is only to allow arrays of Derivs_Limiters
     */
    Derivs_Limiter()
    {
        accel = 0;
        lastTime = 0;
        velLimit = 0;
        originalVelLimit = 0;
        accelLimit = 0;
        setDecelLimit(NAN);
        target = 0;
        posMode = true;
        lastTarget = 0;
        targetDelta = 0;
        position = 0;
        lastPos = 0;
        posDelta = 0;
        velocity = 0;
        time = 0;
        preventGoingWrongWay = false;
        preventGoingTooFast = false;
        posLimitLow = 0;
        posLimitHigh = 0;
        maxStoppingDecel = 1;
        positionPointer = NULL;
        velocityPointer = NULL;
        velocityTarget = 0;
    }

    /**
     * @brief  set position and velocity
     * @param  pos: (float) default: 0, ignored if NAN
     * @param  vel: (float) default: 0, ignored if NAN
     * @retval None
     */
    void setPositionVelocity(float pos = 0, float vel = 0)
    {
        if (!isnan(pos))
            position = pos;
        if (!isnan(vel))
            velocity = vel;
    }

    /**
     * @brief  set target and position
     * @param  targ: (float) default: 0, ignored if NAN
     * @param  pos: (float) default: 0, ignored if NAN
     * @retval None
     */
    void setTargetAndPosition(float targ = 0, float pos = 0)
    {
        if (!isnan(targ)) {
            target = targ;
            posMode = true;
        }
        if (!isnan(pos))
            position = pos;
    }

    /**
     * @brief  set position, ignored if NAN
     * @param  pos: (float) default: 0, ignored if NAN
     * @retval (bool) true if position changed
     */
    bool setPosition(float pos = 0)
    {
        if (pos != position) {
            if (!isnan(pos)) {
                position = pos;
                return true;
            }
        }
        return false;
    }

    /**
     * @brief  set velocity
     * @note If you want to switch to velocity control mode look at setVelConstant() and setVelTarget()
     * @param  vel: (float) default: 0, ignored if NAN
     * @retval (bool) true if velocity changed
     */
    bool setVelocity(float vel = 0)
    {
        if (vel != velocity) {
            if (!isnan(vel)) {
                velocity = vel;
                return true;
            }
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
            originalVelLimit = velLimit;
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
     * @brief  set deceleration limit
     * @param  _decelLimit: (float) deceleration limit, if NAN decelLimit gets set to accelLimit
     * @retval None
     */
    void setDecelLimit(float _decelLimit = NAN)
    {
        if (isnan(_decelLimit)) { // decelLimit defaults to accelLimit
            decelLimit = accelLimit;
        } else {
            decelLimit = abs(_decelLimit);
        }
    }

    /**
     * @brief combines setAccelLimit() with setDecelLimit()
     * @param  _accelLimit:
     * @param  _decelLimit:
     * @retval None
     */
    void setAccelAndDecelLimits(float _accelLimit, float _decelLimit = NAN)
    {
        setAccelLimit(_accelLimit);
        setDecelLimit(_decelLimit);
    }

    /**
     * @brief  set velocity and acceleration limits
     * @param  velLim: (float) velocity limit
     * @param  accLim: (float) acceleration limit
     * @param  decLim: (float) deceleration limit, set NAN to set equal to acceleration limit
     * @retval None
     */
    void setVelAccelLimits(float velLim, float accLim, float decLim = NAN)
    {
        setVelLimit(velLim);
        setAccelLimit(accLim);
        setDecelLimit(decLim);
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
     * @brief  get deceleration limit setting
     * @retval  (float)
     */
    float getDecelLimit()
    {
        return decelLimit;
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
     * @brief  set setting for how many times accelLimit can be used to stop in time for target position
     * @param  _maxStoppingDecel: (float) must be >=1.0, can be INFINITY
     * @retval None
     */
    void setMaxStoppingDecel(float _maxStoppingDecel)
    {
        maxStoppingDecel = max(_maxStoppingDecel, (float)1.0);
    }

    /**
     * @brief  get setting for how many times accelLimit can be used to stop in time for target position
     * @retval (float)
     */
    float getMaxStoppingDecel()
    {
        return maxStoppingDecel;
    }

    /**
     * @brief  get the lower boundary for position
     * @retval (float)
     */
    float getLowPosLimit()
    {
        return posLimitLow;
    }

    /**
     * @brief  get the higher boundary for position
     * @retval (float)
     */
    float getHighPosLimit()
    {
        return posLimitHigh;
    }

    /**
     * @brief  set the lower boundary for position
     * @note must be lower than highPosLimit
     * @param  lowLimit: (float), -INFINITY means no limit
     * @retval (bool) did boundary change (was it valid)
     */
    bool setLowPosLimit(float lowLimit)
    {
        if (lowLimit < posLimitHigh) {
            posLimitLow = lowLimit;
            return true;
        }
        return false;
    }

    /**
     * @brief  set the higher boundary for position
     * @note must be higher than lowPosLimit
     * @param  highLimit: (float), INFINITY means no limit
     * @retval (bool) did boundary change (was it valid)
     */
    bool setHighPosLimit(float highLimit)
    {
        if (highLimit > posLimitLow) {
            posLimitHigh = highLimit;
            return true;
        }
        return false;
    }

    /**
     * @brief  set the boundaries for position
     * @param  lowLimit: (float)
     * @param  highLimit: (float)
     * @retval None
     */
    void setPosLimits(float lowLimit, float highLimit)
    {
        setLowPosLimit(lowLimit);
        setHighPosLimit(highLimit);
    }

    /**
     * @brief  set target position (doesn't run calculation, make sure to run calc() yourself)
     * @param  _target: (float) position, ignored if NAN
     * @retval  (bool) position==target
     */
    bool setTarget(float _target)
    {
        if (!isnan(_target)) {
            target = _target;
            posMode = true;
        }
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
     * @brief  set position and target to a value
     * @param  targPos: (float)
     * @retval None
     */
    void setPositionAndTarget(float targPos)
    {
        setPosition(targPos);
        setTarget(targPos);
    }

    /**
     * @brief  set position and target to position + increment
     * @param  increment: (float)
     * @retval None
     */
    void jogPosition(float increment)
    {
        velocity = 0;
        setPositionAndTarget(position + increment);
    }

    /**
     * @brief If calc hasn't been run for a while, use this before starting to use it again to protect from large jumps.
     * @retval None
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
        return targetDelta;
    }

    /**
     * @brief  what was target in the most recent run of calc()
     * @retval  (float)
     */
    float getLastTarget()
    {
        return lastTarget;
    }

    /**
     * @brief  returns the change in position from the most recent run of calc()
     * @retval  (float)
     */
    float getPositionDelta()
    {
        return posDelta;
    }

    /**
     * @brief  what was position in the most recent run of calc(), can be used to see if position was changed outside of calc()
     * @retval  (float)
     */
    float getLastPosition()
    {
        return lastPos;
    }

    /**
     * @brief  how fast was target changing (distance/time)
     * @note   returns 0 if time is 0
     * @retval (float)
     */
    float getTargetDeltaPerTime()
    {
        if (time > 0)
            return targetDelta / time;
        return 0;
    }

    /**
     * @brief  set pointer to an external variable that will be read and modified during calc as position
     * @note   set to NULL to not use, set to variable with setPositionPointer(&variable)
     * @param  _positionPointer: (float*)
     * @retval None
     */
    void setPositionPointer(float* _positionPointer)
    {
        positionPointer = _positionPointer;
    }

    /**
     * @brief  set pointer to an external variable that will be read and modified during calc as velocity
     * @note   set to NULL to not use,  set to variable with setVelocityPointer(&variable)
     * @param  _velocityPointer: (float*)
     * @retval None
     */
    void setVelocityPointer(float* _velocityPointer)
    {
        velocityPointer = _velocityPointer;
    }

    /**
     * @brief  sets value of preventGoingWrongWay, true = immediately set velocity to zero if moving away from target, false = stay under accel limit
     * @param  _preventGoingWrongWay: (bool)
     * @retval None
     */
    void setPreventGoingWrongWay(bool _preventGoingWrongWay)
    {
        preventGoingWrongWay = _preventGoingWrongWay;
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
     * @brief  sets value of preventGoingTooFast, true = constrain velocity to velLimit, false decelerate at accelLimit to velLimit
     * @param  _preventGoingTooFast: (bool)
     * @retval None
     */
    void setPreventGoingTooFast(bool _preventGoingTooFast)
    {
        preventGoingTooFast = _preventGoingTooFast;
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
     * @brief  returns target - position
     * @note use abs(distToTarget()) if you don't care about direction
     * @retval (float)
     */
    float distToTarget()
    {
        return target - position;
    }

    /**
     * @brief  switch to velocity mode, and set velocity immediately to a constant value.
     * @param  vel: (float)
     * @retval None
     */
    void setVelConstant(float vel)
    {
        if (isnan(vel)) {
            return;
        }
        posMode = false;
        velocity = vel;
        velocityTarget = vel;
    }

    /**
     * @brief  switch to velocity mode, and set a target velocity that the target should go towards limited by accelLimit
     * @param  vel: (float)
     * @retval None
     */
    void setVelTarget(float vel)
    {
        if (isnan(vel)) {
            return;
        }
        posMode = false;
        velocityTarget = vel;
    }

    /**
     * @brief  true if in position target mode, false if in velocity target mode
     * @retval (bool)
     */
    bool isPosModeNotVelocity()
    {
        return posMode;
    }

    /**
     * @brief  get the target velocity used by the velocity control mode
     * @retval (float)
     */
    float getVelTarget()
    {
        return velocityTarget;
    }

    /**
     * @brief  resets the velocity limit to the value set in the constructor or setVelLimit()
     * @note may be useful, since setTargetAndVelLimitForTimedMove and setVelLimitForTimedMove change the velocity limit
     * @retval None
     */
    void resetVelLimitToOriginal()
    {
        velLimit = originalVelLimit;
    }

    /**
     * @brief  This function changes velLimit so that a move of a specified distance takes the specified time (if possible given acceleration limit)
     * @note   using this function changes the value of velLimit from whatever you set it to when you created the Derivs_Limiter object
     * @param  _dist: (float) how far you want to move
     * @param  _time: (float) time in seconds that you would like it to take to move the given distance
     * @param  _maxVel: (float, optional, default=NAN) maximum allowable velocity, if the required velocity exceeds this the function returns false, if NAN the velocity limit set in the constructor or setVelLimit() is used
     * @retval (bool) true if move possible within time given acceleration limit, false if not possible (and nothing is changed)
     */
    boolean setVelLimitForTimedMove(float _dist, float _time, float _maxVel = NAN)
    {
        if (isnan(_dist) || isnan(_time)) {
            return false;
        }
        _dist = abs(_dist);
        _time = abs(_time);
        if (isnan(_maxVel))
            _maxVel = originalVelLimit;
        float tempVelLimit;
        if (accelLimit == INFINITY && decelLimit == INFINITY)
            tempVelLimit = _dist / _time;
        else {
            float acc;
            if (accelLimit == INFINITY && decelLimit != INFINITY)
                acc = decelLimit * 2;
            else if (accelLimit != INFINITY && decelLimit == INFINITY)
                acc = accelLimit * 2;
            else
                acc = sqrt(decelLimit * accelLimit); // find single acceleration that takes equivalent time to the two different limits.
            tempVelLimit = (-0.5 * acc * (-_time + sqrt(sq(_time) - 4 * _dist / acc)));
        }
        boolean possible = !isnan(tempVelLimit) && tempVelLimit <= abs(_maxVel); // nan check, speed check
        if (possible) {
            velLimit = tempVelLimit;
        }
        return possible;
    }

    /**
     * @brief  This function changes velLimit so that a move to the specified target position takes the specified time (if possible given acceleration limit)
     * @note   using this function changes the value of velLimit from whatever you set it to when you created the Derivs_Limiter object
     * @param  _target: (float) position you'd like to move to
     * @param  _time: (float) how long you would like the movement to take
     * @param  _maxVel: (float, optional, default=NAN) maximum allowable velocity, if the required velocity exceeds this the function returns false, if NAN the velocity limit set in the constructor or setVelLimit() is used
     * @retval (bool) true if move possible within time given acceleration limit, false if not possible (and nothing is changed)
     */
    boolean setTargetAndVelLimitForTimedMove(float _target, float _time, float _maxVel = NAN)
    {
        boolean ret = setVelLimitForTimedMove(_target - position, _time, _maxVel);
        if (ret) {
            target = _target;
            posMode = true;
        }
        return ret;
    }

    /**
     * @brief  This function changes velLimit so that a move to the specified target position takes the specified time if possible given acceleration limit, and if not possible resets the velocity limit to the original value (or _maxVel if not NAN) and goes to the target at that speed instead
     * @note   using this function changes the value of velLimit from whatever you set it to when you created the Derivs_Limiter object
     * @param  _target: (float) position you'd like to move to
     * @param  _time: (float) how long you would like the movement to take
     * @param  _maxVel: (float, optional, default=NAN) maximum allowable velocity, if the required velocity exceeds this the function returns false, if NAN the velocity limit set in the constructor or setVelLimit() is used
     * @retval (bool) true if move possible within time given acceleration limit, false if not possible (and move happens with maxVel instead but will not complete in time)
     */
    boolean setTargetTimedMovePreferred(float _target, float _time, float _maxVel = NAN)
    {
        boolean ret = setVelLimitForTimedMove(_target - position, _time, _maxVel);
        if (ret) {
            target = _target;
            posMode = true;
        } else { // not possible in time given acceleration
            if (isnan(_maxVel))
                resetVelLimitToOriginal();
            else
                velLimit = _maxVel;
            target = _target;
            posMode = true;
        }
        return ret;
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
     * @param  _target: set the target position, ignored if NAN
     * @retval (float) position
     */
    float calc(float _target)
    {
        if (!isnan(_target)) {
            target = _target;
            posMode = true;
        }
        return _calc();
    }

protected:
    /**
     * @brief  this is where the actual code is
     * @retval (float) position
     */
    virtual float _calc()
    {
        if (positionPointer && !isnan(*positionPointer))
            position = *positionPointer;

        if (velocityPointer && !isnan(*velocityPointer))
            velocity = *velocityPointer;

        time = (micros() - lastTime) / 1000000.0;
        if (lastTime == 0) {
            time = 0; // in case there's a delay between starting the program and the first calculation avoid jump at start
            lastTime = micros();
        }
        if (time == 0) {
            return position;
        }
        lastTime = micros();

        // constrain positions within limits
        if (position > posLimitHigh) {
            position = posLimitHigh;
            velocity = 0;
        } else if (position < posLimitLow) {
            position = posLimitLow;
            velocity = 0;
        }
        target = constrain(target, posLimitLow, posLimitHigh);

        targetDelta = target - lastTarget;
        lastTarget = target;

        if (preventGoingTooFast) {
            velocity = constrain(velocity, -velLimit, velLimit);
        }
        if (posMode) {
            if (preventGoingWrongWay && velocity != 0 && target != position && ((velocity > 0) != (target - position > 0))) { // going the wrong way
                velocity = 0;
            }

            if (velocity == 0 && position == target) { // if stopped at the target, no calculations are needed
                accel = 0;
                return position;
            }

            if (velocity != 0 && target != position && (velocity > 0) == (target - position > 0)
                && (abs(position - target) - abs(velocity * (time)) <= sq(velocity) / 2.0 / decelLimit)) {
                // predicted to be too close next time, decel now.
                if (abs(position - target) <= abs(velocity * time) && (abs(velocity) <= decelLimit * maxStoppingDecel * time)) { // close enough and slow enough, just stop
                    accel = 0;
                    velocity = 0;
                    position = target;
                } else { // decel
                    accel = -sq(velocity) / 2.0 / (target - (position));
                    accel = constrain(accel, -decelLimit * maxStoppingDecel, decelLimit * maxStoppingDecel);
                    velocity += accel * time;
                    position += velocity * time;
                }
            } else if (velocity != 0 && target != position && (velocity > 0) != (target - position > 0)) { // if going wrong way, decel
                accel = ((target - position > 0) ? decelLimit : -decelLimit);
                velocity += accel * time;
                if (velocity != 0 && (velocity > 0) == (target - position > 0)) { // switched direction, stop at zero velocity, in case accel is lower
                    velocity = 0;
                    accel = 0;
                } else {
                    position += velocity * time;
                }
            } else if (abs(velocity) < velLimit) { // too slow, speed up
                float tempVelocity = velocity;
                accel = (position > target) ? -accelLimit : accelLimit;
                velocity += accel * time;
                velocity = constrain(velocity, -velLimit, velLimit);
                float maxSpeedThatCanBeStopped = sqrt(2 * (decelLimit)*abs(position - target)); // v^2 = u^2 + 2as
                velocity = constrain(velocity, -maxSpeedThatCanBeStopped, maxSpeedThatCanBeStopped);
                accel = (velocity - tempVelocity) / time;
                position += velocity * time;
                if (abs(position - target) <= abs(velocity * time) && (abs(velocity) <= decelLimit * maxStoppingDecel * time)) { // close enough and slow enough, just stop
                    accel = 0;
                    velocity = 0;
                    position = target;
                }
            } else if (abs(velocity) > velLimit) { // too fast, slow down
                boolean velPositive = (velocity > 0);
                float tempVelocity = velocity;
                velocity += velPositive ? -decelLimit * time : decelLimit * time;
                if (velPositive) {
                    if (velocity < velLimit) {
                        velocity = velLimit;
                    }
                } else { // vel negative
                    if (velocity > -velLimit) {
                        velocity = -velLimit;
                    }
                }

                accel = (velocity - tempVelocity) / time;
                position += velocity * time;
            } else { // coast, no accel
                accel = 0;
                position += velocity * time;
            }
        } else { // not pos mode, vel mode
            float tempVelocity = velocity;
            velocityTarget = constrain(velocityTarget, -velLimit, velLimit);
            if (preventGoingWrongWay && velocity != 0 && velocityTarget != 0 && (velocity > 0) != (velocityTarget > 0)) {
                velocity = 0;
            }
            if (velocity != velocityTarget) {
                if (velocity == 0) {
                    velocity += constrain(velocityTarget - velocity, -accelLimit * time, accelLimit * time);
                } else if (velocity > 0) {
                    velocity += constrain(velocityTarget - velocity, -decelLimit * time, accelLimit * time);
                    if (velocity < 0) { // prevent decel from crossing zero and causing accel
                        velocity = 0;
                    }
                } else { // velocity < 0
                    velocity += constrain(velocityTarget - velocity, -accelLimit * time, decelLimit * time);
                    if (velocity > 0) { // prevent decel from crossing zero and causing accel
                        velocity = 0;
                    }
                }
            }
            accel = (velocity - tempVelocity) / time;
            position += velocity * time;
        }

        if (positionPointer)
            *positionPointer = position;
        if (velocityPointer)
            *velocityPointer = velocity;

        posDelta = position - lastPos;
        lastPos = position;

        return position;
    }
};
#endif
