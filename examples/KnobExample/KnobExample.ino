/**
 * knobExample, an example for the Derivs_Limiter library
 * https://github.com/joshua-8/Derivs_Limiter
 *
 * Connect a servo to pin 9, and a potentiometer to pin A0
 *
 * More documentation can be found here: https://joshua-8.github.io/Derivs_Limiter/class_derivs___limiter.html
 */
#include <Arduino.h>
#include <Derivs_Limiter.h>
#include <Servo.h>

Derivs_Limiter limiter = Derivs_Limiter(50, 100, 50); // velocityLimit, accelerationLimit, decelerationLimit
Servo myServo;

void setup()
{
    Serial.begin(115200);
    myServo.attach(9);
    limiter.setPreventGoingWrongWay(false);
    limiter.setPosLimits(0, 180);
}
void loop()
{
    limiter.setTarget(map(analogRead(A0), 0, 1023, 0, 1800) / 10.0);

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
