/*
 Name:		Wemos_UNO.ino
 Created:	12/7/2021 11:27:30 AM
 Author:	kh
*/



#include "AES.h"
#include "SinWaveGenerator.h"


void InitTestPattern() {
    for (int i = 0; i < NUM_FRAMES_TO_SEND; i++) {
        AES.WriteFrame(i, AES.y, i << 8);
    }

    AES.WriteFrame(191, AES.y, 0x5A5A00);
}


void setup()
{
    Serial.begin(921600);

    Serial.println("Starting up");



    //InitTestPattern();
}

void loop()
{
    if (digitalRead(FrameFlag)) {

        digitalWrite(FrameFlag, LOW);
        Generator.GetSamples();
    }
    // put your own loop code here
}