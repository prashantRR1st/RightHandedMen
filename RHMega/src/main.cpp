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


/**
 * VEX Robotics Victor 888 Motor Controller The Vex Robotics Victor 884 Motor Controller can also be
 * used with this class but may need to be calibrated per the Victor 884 user manual.
 *
 * <p>Note that the Victor uses the following bounds for PWM values. These values were determined
 * empirically and optimized for the Victor 888. These values should work reasonably well for Victor
 * 884 controllers also but if users experience issues such as asymmetric behavior around the
 * deadband or inability to saturate the controller in either direction, calibration is recommended.
 * The calibration procedure can be found in the Victor 884 User Manual available from VEX Robotics:
 * http://content.vexrobotics.com/docs/ifi-v884-users-manual-9-25-06.pdf
 *
 * <ul>
 *   <li>2.027ms = full "forward"
 *   <li>1.525ms = the "high end" of the deadband range
 *   <li>1.507ms = center of the deadband range (off)
 *   <li>1.490ms = the "low end" of the deadband range
 *   <li>1.026ms = full "reverse"
 * </ul>
 */

void configMotor(){
    
}

void writeMotor(int speed){

}