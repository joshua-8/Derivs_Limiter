/**
 * SmoothServo, an example for the Derivs_Limiter library
 * https://github.com/joshua-8/Derivs_Limiter
 *
 * Connect a servo to power and pin 9,
 * and/or open the serial plotter to see the smoothed position values.
 *
 * More documentation can be found here: https://joshua-8.github.io/Derivs_Limiter/class_derivs___limiter.html
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>
#include <Servo.h>

Derivs_Limiter limiter = Derivs_Limiter(100, 100, 50); // velocityLimit, accelerationLimit, decelerationLimit
Servo myServo;
void setup()
{
    Serial.begin(115200);
    Serial.println("position,velocity,acceleration");
    myServo.attach(9);
}
void loop()
{
    if (millis() % 10000 < 5000) { // toggles every 5 seconds
        myServo.write(limiter.calc(0));
    } else {
        myServo.write(limiter.calc(180));
    }
    Serial.print(limiter.getPosition());
    Serial.print(",");
    Serial.print(limiter.getVelocity());
    Serial.print(",");
    Serial.print(limiter.getAcceleration());
    Serial.println();
    delay(25);
}
