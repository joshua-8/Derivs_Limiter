/**
 * knobExample, an example for the Derivs_Limiter library
 * https://github.com/joshua-8/Derivs_Limiter
 *
 * Connect a servo to power and pin 9, and a potentiometer to pin A0
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>
#include <Servo.h>

Derivs_Limiter limiter = Derivs_Limiter(50, 75, 25); // velocityLimit, accelerationLimit, decelerationLimit
Servo myServo;

void setup()
{
    Serial.begin(9600);
    myServo.attach(9);
}
void loop()
{
    limiter.setTarget(map(analogRead(A0), 0, 1023, 0, 180));

    myServo.write(limiter.calc());
    Serial.print(limiter.getPosition());
    Serial.print(",");
    Serial.print(limiter.getVelocity());
    Serial.print(",");
    Serial.print(limiter.getAcceleration());
    Serial.print(",");
    Serial.print(limiter.isPosModeNotVelocity());
    Serial.println();
}
