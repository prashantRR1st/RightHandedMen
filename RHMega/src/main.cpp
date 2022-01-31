#include <Arduino.h>

// driver pins
int pulPin = 13; //980 Hz PWM on Mega
int dirPin = 7;
int dc = 200;
int delayTime = 2000;

int encoderOutput;
unsigned long counter = 0; // counting encoder pulses

bool run = false;

void setup(){
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println();
    Serial.println("Starting...");

    //motor driver
    pinMode(pulPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    digitalWrite(dirPin, HIGH);
    //analogWrite(pulPin, 127); // 0 - 0% duty cycle, 255 - 100% duty cycle
}

void loop(){
    // receive command from serial terminal
    if (Serial.available() > 0)    {
        char inByte = Serial.read();
        if (inByte == 'w')            run = true;
        else if (inByte == 's')       run = false;
        else Serial.println(inByte);
    }

    if (run) {
        digitalWrite(pulPin, HIGH);
        delayMicroseconds(dc);
        digitalWrite(pulPin, LOW);
        delayMicroseconds(delayTime - dc);
    }
    else {
        digitalWrite(pulPin, LOW);
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
