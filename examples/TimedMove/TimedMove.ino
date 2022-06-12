/**
 * TimedMove, an example for the Derivs_Limiter library
 * https://github.com/joshua-8/Derivs_Limiter
 *
 * Open the serial monitor to see the smoothed position values.
 *          time, position, velocity, acceleration
 * Every 15 seconds the values should change for 10 seconds.
 *
 * More documentation can be found here: https://joshua-8.github.io/Derivs_Limiter/class_derivs___limiter.html
 */

#include <Arduino.h>
#include <Derivs_Limiter.h>

Derivs_Limiter limiter = Derivs_Limiter(100, 10); // velocityLimit, accelerationLimit, (decelerationLimit)
boolean state = true;
void setup()
{
    Serial.begin(115200);
    Serial.println("time,position,velocity,acceleration");
}
void loop()
{
    if (millis() % 30000 < 15000) { // toggles every 15 seconds
        if (state == true)
            limiter.setTargetAndVelLimitForTimedMove(180, 10);
        state = false;
        limiter.calc();
    } else {
        if (state == false)
            limiter.setTargetAndVelLimitForTimedMove(0, 10);
        state = true;
        limiter.calc();
    }
    Serial.print((millis() % 30000 < 15000) ? (millis() % 30000 / 1000.0) - 10 : ((millis() - 15000) % 30000 / 1000.0) - 10);
    Serial.print(",");
    Serial.print(limiter.getPosition());
    Serial.print(",");
    Serial.print(limiter.getVelocity());
    Serial.print(",");
    Serial.print(limiter.getAcceleration());
    Serial.println();
    delay(20);
}
