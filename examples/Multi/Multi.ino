/*
    This example demonstrates the use of multiple instances of DerivsLimiter.
    Each one calulates an independent motion profile.
    This example uses arrays to avoid duplicating code
    Tested on an Arduino Uno but should work on anything.
    Requires Derivs_Limiter version 3.1.0 or greater

    v1.0 by joshua-8 2022-11-16
*/
#include <Arduino.h>
#include <Derivs_Limiter.h> // https://github.com/joshua-8/Derivs_Limiter
// detailed documentation of Derivs_Limiter can be found here: https://joshua-8.github.io/Derivs_Limiter/class_derivs___limiter.html

const int NUM_DLs = 4; // limited only by the Arduino's memory and the time it takes to compute each Derivs_Limiter

Derivs_Limiter DL[NUM_DLs];

// random settings just as an example of each Derivs_Limiter getting unique settings
float velLimit[NUM_DLs] = { 150, 222.2, 122, 160 };
float accLimit[NUM_DLs] = { 270, 245.5, 250, 340 };

void setup()
{
    Serial.begin(115200);
    for (int i = 0; i < NUM_DLs; i++) {
        DL[i] = Derivs_Limiter(velLimit[i], accLimit[i]); // https://joshua-8.github.io/Derivs_Limiter/class_derivs___limiter.html#a674339d1db4ca3a2358e1d25b9eb6cf4
        // now is a good place to set other settings for each DL
    }
}

void loop()
{
    for (int i = 0; i < NUM_DLs; i++) {

        float targ = ((millis() % (500 + 100 * i)) < (500 + 100 * i) / 2) ? -(i + 1) : (i + 1); // targets are set to be -(i+1) or (i+1) on an interval that is unique for each DL
        DL[i].setTarget(targ); // you could give each DL any target you want it to go to

        DL[i].calc();

        Serial.print(DL[i].getPosition()); // DL[i].getPosition() is the value that could be sent to the i numbered servo
        if (i < NUM_DLs - 1) // add commas between numbers, a newline at the end
            Serial.print(", ");
        else
            Serial.println();
    }
}