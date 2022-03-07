#include <Arduino.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <utility\interrupt_pins.h>
#include <ezButton.h>
#include <Servo.h>
#include <HX711_ADC.h>
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

#define MAX_OUTPUT (1.0)
#define LENGTH_ENC (46552)
#define LENGTH_MM (373.85)
#define POSITION_SCALING_FACTOR (LENGTH_MM / LENGTH_ENC)
#define DEBOUNCE_MS (50)
#define VICTOR_PIN (6)
#define ENC_ORANGE CORE_INT3_PIN
#define ENC_BLUE CORE_INT2_PIN
#define NEAR_LIMIT CORE_INT0_PIN
#define FAR_LIMIT CORE_INT4_PIN
#define HX711_DOUT CORE_INT1_PIN
#define HX711_SCK (5)
#define HX711_CAL_TIME (2000)
#define HX711_CAL_VAL (108.98)
#define SLOW_ZONE (300.0)
#define SLOW_POWER_NEAR (0.11)
#define SLOW_POWER_FAR (0.16)

float loadCellOffset =  75574.25;

bool run = false;

Encoder enc(ENC_ORANGE, ENC_BLUE);
ezButton nearSwitch(NEAR_LIMIT);
ezButton farSwitch(FAR_LIMIT);
Servo motor;
HX711_ADC loadCell(HX711_DOUT, HX711_SCK);

float kP = -0.0009220;
float kI = 0.0;
float kD = 0.0;
float kF = 0.0;
float iZone = 0.0;
float iState = 0.0;
float prev_err = 0.0;
float kMinOutput = -MAX_OUTPUT;
float kMaxOutput = MAX_OUTPUT;
float prev_output = 0.0;

float effectiveMin = kMinOutput;
float effectiveMax = kMaxOutput;

float pos = 0;
float ref = 0;

float open_u = 0.0;
float open_u_rate = 0.015;
float closed_u = 0.0;

float pid_step(float setpoint, float pv);

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

void resetMid()
{
    enc.write(LENGTH_ENC/2);
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

#define VICMAX (2327)
#define VICMAX_DEADBAND (1525)
#define VICMID (1507)
#define VICMIN_DEADBAND (1490)
#define VICMIN (687)

float lerp(float low, float high, float alpha)
{
    return low * (1.0 - alpha) + high * alpha;
}

float unlerp(float low, float high, float val)
{
    return (val - low) /  (high - low);
}

float remap(float low_old, float high_old, float val, float low, float high){
    return lerp(low, high, unlerp(low_old, high_old, val));
}

void processOutput(float output, float pos){
    nearSwitch.loop();
    farSwitch.loop();

    if (nearSwitch.isPressed())
    {
        restrict_rev();
        resetNear();
        return;
    }
    else if (nearSwitch.isReleased())
    {
        unrestrict_rev();
    }

    if (farSwitch.isPressed())
    {
        restrict_fwd();
        resetFar();
        return;
    }
    else if (farSwitch.isReleased())
    {
        unrestrict_fwd();
    }

    if(output > 0.0){
        if(pos >= LENGTH_MM - SLOW_ZONE  && pos < LENGTH_MM){
            effectiveMax = remap(LENGTH_MM - SLOW_ZONE, LENGTH_MM, pos, kMaxOutput, SLOW_POWER_FAR);
        } else {
            unrestrict_fwd();
        }
    } else {
        if(pos <= SLOW_ZONE && pos > 0.0){
            effectiveMin = remap(0.0, SLOW_ZONE, pos, -SLOW_POWER_NEAR, kMinOutput);
        }else {
            unrestrict_rev();
        }
    }
}

#define RATE_LIMIT (2.0)

float prev_out = 0.0;
void writeMotor(float speed)
{
    int res = VICMID;
    float out = clampOutput(speed);

    if(out-prev_out > RATE_LIMIT) out = prev_out+RATE_LIMIT;
    else if(prev_out-out > RATE_LIMIT) out = prev_out-RATE_LIMIT;

    if (out > 0.0)
    {
        res = floor(lerp(VICMAX_DEADBAND, VICMAX, out));
    }
    else if (out < 0.0)
    {
        res = floor(lerp(VICMIN_DEADBAND, VICMIN, -out));
    }
    prev_out = out;
    motor.writeMicroseconds(res);
}
void configMotor()
{
    motor.attach(VICTOR_PIN);

    writeMotor(0.0);
}

volatile bool newLoadCellData;

void loadCellReadyISR() {
  if (loadCell.update()) {
    newLoadCellData = true;
  }
}
float loadCellVal = 0.0;

void zeroLoadCell(){
    loadCellOffset += loadCellVal;
}

void pollLoadCell(){
    loadCellReadyISR();
    if(newLoadCellData){
        loadCellVal = loadCell.getData() - loadCellOffset;
        newLoadCellData = false;
    }
}

void configLoadCell()
{
    newLoadCellData = false;
    loadCell.begin();
    loadCell.start(HX711_CAL_TIME, false);
    if (loadCell.getTareTimeoutFlag())
    {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        while (1);
    }
    else
    {
        loadCell.setCalFactor(HX711_CAL_VAL);
    }
    //attachInterrupt(digitalPinToInterrupt(HX711_DOUT), loadCellReadyISR, FALLING);

    delay(500);
    pollLoadCell();
    zeroLoadCell();
}


bool open;

float targetLoad = 1500;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);

    nearSwitch.setDebounceTime(DEBOUNCE_MS);
    farSwitch.setDebounceTime(DEBOUNCE_MS);

    configMotor();
    configLoadCell();

    Serial.println("Starting...");
    open = true;
}

float tune_rate = 0.00001;

void loop()
{
    if(open) processOutput(open_u, pos);
    else processOutput(closed_u, pos);
    
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
        else if (inByte == 'm')
            resetMid();
        else if (inByte == 'p'){
            if(open) Serial.println(open_u);
            else Serial.println(closed_u);
        }
        else if (inByte == 'e')
            Serial.println(pos);
        else if (inByte == 'q')
            Serial.println(loadCellVal);
        else if (inByte == 'z')
            zeroLoadCell();
        else if (inByte == 'o')
            open = true;
        else if (inByte == 'c')
            open = false;
        else if (inByte == 't')
            kP -= tune_rate;
        else if (inByte == 'u')
            kP += tune_rate;
        else if (inByte == 'y')
            kP = 0;
        else if (inByte == 'i')
            Serial.println(kP, 6);
        else
            Serial.println(inByte);
    }

    pos = position();
    pollLoadCell();

    closed_u = pid_step(targetLoad, loadCellVal);

    if (run){
        if(open) writeMotor(open_u);
        else writeMotor(closed_u);
    } else {
        writeMotor(0.0);
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
