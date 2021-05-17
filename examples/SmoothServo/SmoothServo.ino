/**
 * SmoothServo, an example for the Derivs_Limiter library
 * https://github.com/joshua-8/Derivs_Limiter
 * 
 * Connect a servo to power and pin 9,
 * and/or open the serial plotter to see the smoothed position values.
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>
#include <Servo.h>

Derivs_Limiter limiter = Derivs_Limiter(100, 75); // velocityLimit, accelerationLimit
Servo myServo;
void setup()
{
    Serial.begin(9600);
    myServo.attach(9);
}
void loop()
{
    if (millis() % 10000 < 5000) { //toggles every 5 seconds
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
}
