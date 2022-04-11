int limit_start_pin = 8;
int limit_end_pin = 7;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(limit_start_pin, INPUT);
  pinMode(limit_end_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  char strBuf[50];
  sprintf(strBuf, "limit_start = %d \tlimit_end = %d", digitalRead(limit_start_pin), digitalRead(limit_end_pin));
  Serial.println(strBuf);
}
