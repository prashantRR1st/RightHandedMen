#include <Arduino.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <utility\interrupt_pins.h>
#include <ezButton.h>
#include <Servo.h>

// No longer accurate
#define LENGTH_ENC (46552)
#define LENGTH_MM (450.85)
#define POSITION_SCALING_FACTOR (LENGTH_MM / LENGTH_ENC)
#define DEBOUNCE_MS (50)
#define VICTOR_PIN (9)
#define VICTOR_MID (90)
#define VICTOR_AMPLTIUDE (90)

bool run = false;

Encoder enc(CORE_INT0_PIN, CORE_INT1_PIN);
ezButton nearSwitch(CORE_INT2_PIN);
ezButton farSwitch(CORE_INT3_PIN);
Servo motor;

float kP = 0.0;
float kI = 0.0;
float kD = 0.0;
float kF = 0.0;
float iZone = 0.0;
float iState = 0.0;
float prev_err = 0.0;
float kMinOutput = -1.0;
float kMaxOutput = 1.0;
float prev_output = 0.0;

float effectiveMin = kMinOutput;
float effectiveMax = kMaxOutput;

float clampOutput(float output)
{
    return fminf(fmaxf(output, effectiveMin), effectiveMax);
}

float position()
{
    return POSITION_SCALING_FACTOR * enc.read();
}

void restrict_fwd()
{
    effectiveMax = 0.0;
}

void restrict_rev()
{
    effectiveMin = 0.0;
}

void unrestrict_fwd()
{
    effectiveMax = kMaxOutput;
}

void unrestrict_rev()
{
    effectiveMin = kMinOutput;
}

void resetNear()
{
    enc.write(0);
}

void resetFar()
{
    enc.write(LENGTH_ENC);
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

#define VICMAX (2390)
#define VICMAX_THRES (1520)
#define VICMID (1465)
#define VICMIN_THRES (1490)
#define VICMIN (497)

int lerp(int low, int high, float alpha){
    return low * (1.0 - alpha) + high * alpha;
}

void writeMotor(float speed)
{
    int res = VICMID;
    float out = clampOutput(speed);

    if(out > 0.0){
        res = lerp(VICMAX_THRES, VICMAX, out);
    } else if (out < 0.0) {
        res = lerp(VICMIN_THRES, VICMIN, -out);
    }

    motor.writeMicroseconds(res);
}
void configMotor()
{
    motor.attach(VICTOR_PIN);

    writeMotor(0.0);
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println();
    Serial.println("Starting...");

    nearSwitch.setDebounceTime(DEBOUNCE_MS);
    farSwitch.setDebounceTime(DEBOUNCE_MS);

    configMotor();
}

float oldPos = -999.0;
float pos = 0;
float ref = 0;

float open_u = 0.0;
float open_u_rate = 0.015;

void loop()
{
    nearSwitch.loop();
    farSwitch.loop();

    if (nearSwitch.isPressed())
    {
        restrict_rev();
        resetNear();
    }
    else if (nearSwitch.isReleased())
    {
        unrestrict_rev();
    }

    if (farSwitch.isPressed())
    {
        restrict_fwd();
        resetFar();
    }
    else if (farSwitch.isReleased())
    {
        unrestrict_fwd();
    }

    // receive command from serial terminal
    if (Serial.available() > 0)
    {
        char inByte = Serial.read();
        if (inByte == 'w')
        {
            open_u = 0;
            run = true;
        }
        else if (inByte == 'a')
            open_u -= open_u_rate;
        else if (inByte == 's')
            run = false;
        else if (inByte == 'd')
            open_u += open_u_rate;
        else if (inByte == 'n')
            resetNear();
        else if (inByte == 'f')
            resetFar();
        else if (inByte == 'p')
            Serial.println(open_u);
        else
            Serial.println(inByte);
    }

    open_u = clampOutput(open_u);

    pos = position();
    if (pos != oldPos)
    {
        oldPos = pos;
        Serial.println(pos);
    }

    if(run) writeMotor(open_u);
    else writeMotor(0.0);

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

float pid_step(float setpoint, float pv)
{
    float error = setpoint - pv;

    float p = error * kP;

    if (fabsf(error) <= iZone || iZone == 0.0f)
    {
        iState = iState + (error * kI);
    }
    else
    {
        iState = 0;
    }

    float d = (error - prev_err);
    prev_err = error;
    d *= kD;

    float f = setpoint * kF;

    float output = p + iState + d + f;
    prev_output = output;
    return output;
}
