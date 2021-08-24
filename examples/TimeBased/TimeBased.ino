/**
 * TimeBased, an example for the Derivs_Limiter library
 * https://github.com/joshua-8/Derivs_Limiter
 * 
 * Open the serial plotter to see the smoothed position values.
 * Every 15 seconds the values should move for 10 seconds.
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>

Derivs_Limiter limiter = Derivs_Limiter(14, 20); // velocityLimit, accelerationLimit

boolean state = true;
void setup()
{
    Serial.begin(9600);
}
void loop()
{
    if (millis() % 30000 < 15000) { //toggles every 15 seconds
        if (state == true) //important to only start timed move once
            limiter.setTargetAndVelLimitForTimedMove(0, 10);
        state = false;
        limiter.calc();
    } else {
        if (state == false) //important to only start timed move once
            limiter.setTargetAndVelLimitForTimedMove(15, 10);
        state = true;
        limiter.calc();
    }
    Serial.print(limiter.getPosition(), 4);
    Serial.print(",");
    Serial.print(limiter.getVelocity());
    Serial.print(",");
    Serial.print(limiter.getAcceleration());
    Serial.println();
}
