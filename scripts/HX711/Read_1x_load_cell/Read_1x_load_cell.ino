/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout_a = 4; //mcu > HX711 dout pin (apparatus)
const int HX711_sck_a = 5; //mcu > HX711 sck pin (apparatus)
const int HX711_dout_t = 6; //mcu > HX711 dout pin (test)
const int HX711_sck_t = 7; //mcu > HX711 sck pin (test)

//HX711 constructor:
HX711_ADC LoadCell_a(HX711_dout_a, HX711_sck_a);
HX711_ADC LoadCell_t(HX711_dout_t, HX711_sck_t);

const int calVal_eepromAddress = 0; // 0 - cal factor for test rig, 1 - cal factor for apparatus
unsigned long t = 0;
// default start with test load cell
bool load_cell = 0; // 0 - test, 1 - apparatus

void setup() {
  Serial.begin(57600); delay(10);

  LoadCell_a.begin();
  LoadCell_t.begin();
  
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  
  float calibrationValue_a; // calibration value (see example file "Calibration.ino")
  float calibrationValue_t;
  
  calibrationValue_a = 108.98; // uncomment this if you want to set the calibration value in the sketch
  calibrationValue_t = 106.99;
  
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAddress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom
  /*Serial.print("Value a: ");
  Serial.println(calibrationValue_a);
  Serial.print("Value t: ");
  Serial.println(calibrationValue_t);*/

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell_a.start(stabilizingtime, _tare);
  LoadCell_t.start(stabilizingtime, _tare);
  
  if (LoadCell_a.getTareTimeoutFlag()) {
    //Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell_a.setCalFactor(calibrationValue_a); // set calibration value (float)
    //Serial.println("Startup is complete");
  }

  if (LoadCell_t.getTareTimeoutFlag()) {
    //Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell_t.setCalFactor(calibrationValue_t); // set calibration value (float)
    //Serial.println("Startup is complete");
  }
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // get apparatus or test selection
  // receive command from serial terminal
  // send 't' or 'a':
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 'a') load_cell = 1;
    else if (inByte == 't') load_cell = 0;
  }
  
  if (load_cell == 1){
    // check for new data/start next conversion:
    if (LoadCell_a.update()) newDataReady = true;
    
    // get smoothed value from the dataset:
    if (newDataReady) {
      if (millis() > t + serialPrintInterval) {
        float i = LoadCell_a.getData();
        Serial.print("Load_cell_a output val: ");
        Serial.println(i);
        newDataReady = 0;
        t = millis();
      }    
    }
  } else if (load_cell == 0){
    // check for new data/start next conversion:
    if (LoadCell_t.update()) newDataReady = true;
    
    // get smoothed value from the dataset:
    if (newDataReady) {
      if (millis() > t + serialPrintInterval) {
        float i = LoadCell_t.getData();
        Serial.print("Load_cell_t output val: ");
        Serial.println(i);
        newDataReady = 0;
        t = millis();
      }    
    }
  }
}
