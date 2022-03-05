#include <Arduino.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <utility\interrupt_pins.h>
#include <ezButton.h>

// No longer accurate
#define LENGTH (51827)
#define DEBOUNCE_MS (50)

// driver pins
int dc = 100;
int delayTime = 100;

int encoderOutput;
unsigned long counter = 0; // counting encoder pulses

bool run = false;

Encoder enc(CORE_INT0_PIN, CORE_INT1_PIN);
ezButton nearSwitch(CORE_INT2_PIN);
ezButton farSwitch(CORE_INT3_PIN);

long position(){
    return enc.read();
}


void setup(){
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println();
    Serial.println("Starting...");

    nearSwitch.setDebounceTime(DEBOUNCE_MS);
    farSwitch.setDebounceTime(DEBOUNCE_MS);
    
}

long oldPos  = -999;
long pos = 0;
long ref = 0;

void loop(){
    nearSwitch.loop();
    farSwitch.loop();

    if(nearSwitch.isPressed()){
        enc.write(0);
    }
    if(farSwitch.isPressed()){
        enc.write(LENGTH);
    }

    // receive command from serial terminal
    if (Serial.available() > 0)    {
        char inByte = Serial.read();
        if (inByte == 'w')            run = true;
        else if (inByte == 'a')       run = true;
        else if (inByte == 's')       run = false;
        else if (inByte == 'd')       run = false;
        else if (inByte == 'r')       enc.write(0);
        else Serial.println(inByte);
    }

    pos = position();
    if (pos != oldPos) {
        oldPos = pos;
        Serial.println(pos);
    }
    

    // Read encoders
    /*Serial.println("Serial 1 avail");
    Serial.println(Serial1.available());
    Serial.println("Serial 0 avail");
    Serial.println(Serial.available());
    delay(1000);
    if (Serial1.available() > 0) {
        encoderOutput = Serial1.read();
    }
  
    Serial.println(encoderOutput);*/
}
