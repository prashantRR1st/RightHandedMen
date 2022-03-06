// Motor encoder output pulse per rotation (change as required)
#define ENC_COUNT_REV 250

// Encoder output to Arduino Interrupt pin
#define CHANNEL_A 3 // internal interrupt #1
#define CHANNEL_B 8 

// encoder pulse counter
volatile long pulse_count = 0;

// encoder direction
// CCW = 0, CW = 1
volatile bool encoder_dir = 0;

// State of each channel (ISR)
volatile bool Ch_A = 0;
volatile bool Ch_B = 0;
// volatile bool Ch_A_prev = 0;
volatile bool Ch_B_prev = 0;

// One-second interval for measurements
int interval = 1000;

// Counters for milliseconds during interval
long previousMillis = 0;
long currentMillis = 0;

// Variable for RPM measuerment
int rpm = 0;

// Variable for PWM motor speed output
int motorPwm = 0;

void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600); 
  
  // Set encoder as input with internal pullup  
  pinMode(CHANNEL_A, INPUT_PULLUP); 
  pinMode(CHANNEL_B, INPUT_PULLUP);

  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(CHANNEL_A), updateEncoder, RISING);

  // get the initial state of channel B
  Ch_B_prev = digitalRead(CHANNEL_B);
}

void loop()
{
/*
    // Control motor with potentiometer
    motorPwm = map(analogRead(speedcontrol), 0, 1023, 0, 255);
    
    // Write PWM to controller
    analogWrite(PWM, motorPwm);
  
  // Update RPM value every second
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;


    // Calculate RPM
    rpm = (float)(encoderValue * 60 / ENC_COUNT_REV);

    // Only update display when there is a reading
    if (motorPwm > 0 || rpm > 0) {
      Serial.print("PWM VALUE: ");
      Serial.print(motorPwm);
      Serial.print('\t');
      Serial.print(" PULSES: ");
      Serial.print(encoderValue);
      Serial.print('\t');
      Serial.print(" SPEED: ");
      Serial.print(rpm);
      Serial.println(" RPM");
    }
 
    encoderValue = 0;
  }*/
  Serial.print("Encoder direction: ");
  Serial.print(encoder_dir);
  Serial.print('\t');
  Serial.print(" pulse count: ");
  Serial.println(pulse_count);
}
// interrupt A ISR upon Channel A rising edge
// CCW: A leads B (dir = 0)
// CW: A lags B (dir = 1)
void updateEncoder()
{
  // first read channel B value
  Ch_B = digitalRead(CHANNEL_B);

  // check for direction
  if (Ch_B == LOW){
    encoder_dir = 0;
  } else {
    encoder_dir = 1;
  }
  pulse_count++;
}
