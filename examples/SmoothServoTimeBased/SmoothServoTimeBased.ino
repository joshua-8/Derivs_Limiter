/**
 * SmoothServoTimeBased, an example for the Derivs_Limiter library
 * https://github.com/joshua-8/Derivs_Limiter
 *
 * Connect a servo to power and pin 9,
 * and/or open the serial plotter to see the smoothed position values.
 * Every 15 seconds the servo should move for 10 seconds.
 *
 * More documentation can be found here: https://joshua-8.github.io/Derivs_Limiter/class_derivs___limiter.html
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>
#include <Servo.h>

Derivs_Limiter limiter = Derivs_Limiter(100, 10); // velocityLimit, accelerationLimit
Servo myServo;
boolean state = true;
void setup()
{
    Serial.begin(115200);
    myServo.attach(9);
}
void loop()
{
    if (millis() % 40000 < 20000) { // toggles every 20 seconds
        if (state == true)
            limiter.setTargetAndVelLimitForTimedMove(180, 10);
        state = false;
        limiter.calc();
        myServo.write(limiter.getPosition());
    } else {
        if (state == false)
            limiter.setTargetAndVelLimitForTimedMove(0, 15);
        state = true;
        limiter.calc();
        myServo.write(limiter.getPosition());
    }
    Serial.print(limiter.getPosition());
    Serial.print(",");
    Serial.print(limiter.getVelocity());
    Serial.print(",");
    Serial.print(limiter.getAcceleration());
    Serial.println();
    delay(20);
}
