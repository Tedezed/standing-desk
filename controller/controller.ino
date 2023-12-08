#include <EEPROM.h>

// Stop
bool stop_on = false;

// Motor
int pint_int_1 = 2;
int pint_int_2 = 3;
int pint_int_3 = 4;
int pint_int_4 = 5;

// Ultrasonic sensor
int num_shots = 2;
int address_1  = 45;
int address_2  = 85;
int pin_sensor_trig  = 6;
int pin_sensor_echo  = 7;
long duration;
int distance;

// Buttons
int mode = 0;
int max_mode = 1;
int pin_button_0 = 8;
int last_state_pin_button_0 = 1;
int pin_button_1 = 9;
int pin_button_2 = 10;
int pin_button_3 = 11;
int pin_button_4 = 12;

void setup() {
  Serial.begin(9600);
  pinMode(pint_int_1, OUTPUT);
  pinMode(pint_int_2, OUTPUT);
  pinMode(pint_int_3, OUTPUT);
  pinMode(pint_int_4, OUTPUT);
  pinMode(pin_sensor_trig, OUTPUT);
  pinMode(pin_sensor_echo, INPUT);
  pinMode(pin_button_0, INPUT_PULLUP);
  pinMode(pin_button_1, INPUT_PULLUP);
  pinMode(pin_button_2, INPUT_PULLUP);
  pinMode(pin_button_3, INPUT_PULLUP);
  pinMode(pin_button_4, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("- [INFO] Init Arduino motor table control");
  LedOff();
}

void loop () {
  serialMonitorControl();
  buttonControl();
}

void serialMonitorControl () {
  if (Serial.available() > 0) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    Serial.println(s);

    // Sensor
    if (s.equals("u_read")) {
      readDistance();
    }
    if (s.equals("m_read_1")) {
      Serial.println(readIntFromEEPROM(address_1));
    }
    if (s.equals("m_read_2")) {
      Serial.println(readIntFromEEPROM(address_2));
    }
    if (s.equals("m_write_1")) {
      sensorWritePosition(address_1);
    }
    if (s.equals("m_write_2")) {
      sensorWritePosition(address_2);
    }
    if (s.equals("to_position_1")) {
      toPosition(address_1);
    }
    if (s.equals("to_position_2")) {
      toPosition(address_2);
    }

    // Motors
    
    if (s.equals("up1")) {
      Up1();
    }
    if (s.equals("down1")) {
      Down1();
    }
    if (s.equals("up2")) {
      Up2();
    }
    if (s.equals("down2")) {
      Down2();
    }
    if (s.equals("up")) {
      Up();
    }
    if (s.equals("down")) {
      Down();
    }

    if (s.equals("help")) {
      Serial.println("- Commands: up, down, up[number_motor], down[number_motor]");
    }
  }
}

void buttonControl () {

  byte buttonStateButton0 = digitalRead(pin_button_0);
  byte buttonStateButton1 = digitalRead(pin_button_1);
  byte buttonStateButton2 = digitalRead(pin_button_2);
  byte buttonStateButton3 = digitalRead(pin_button_3);
  byte buttonStateButton4 = digitalRead(pin_button_4);


  // Mode 
  if (buttonStateButton0 == LOW) {
    Serial.println("- Button0");
    if (stop_on != true) {
      if (last_state_pin_button_0 == 1)
      {
        mode = mode + 1;
        if (mode > max_mode) {
          mode = 0;
        }
        last_state_pin_button_0 = 0;
      }
      Serial.print("- Mode: ");
      Serial.println(mode);
    }
  } else {
    stop_on = false;
  }
  if (buttonStateButton0 == HIGH) {
   last_state_pin_button_0 = 1;
  }

  // Positions
  
  if (buttonStateButton1 == LOW) {
    Serial.println("- Button1");
    if (mode == 0) {
      toPosition(address_1);
    }
    if (mode == 1) {
      sensorWritePosition(address_1);
    }
  }
  if (buttonStateButton2 == LOW) {
    Serial.println("- Button2");
    if (mode == 0) {
      toPosition(address_2);
    }
    if (mode == 1) {
      sensorWritePosition(address_2);
    }
  }

  // Up&Down
  if (buttonStateButton3 == LOW) {
    Serial.println("- Button3");
    if (mode == 0) {
      Up();
    }
    if (mode == 1) {
      Up2();
    }
  }
  if (buttonStateButton4 == LOW) {
    Serial.println("- Button4");
    if (mode == 0) {
      Down();
    }
    if (mode == 1) {
      Down2();
    }
  }
}

// Ultrasonic sensor

int readDistance() {
  // Clears the pin_sensor_trig
  digitalWrite(pin_sensor_trig, LOW);
  delayMicroseconds(2);
  // Sets the pin_sensor_trig on HIGH state for 10 micro seconds
  digitalWrite(pin_sensor_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_sensor_trig, LOW);
  // Reads the pin_sensor_echo, returns the sound wave travel time in microseconds
  duration = pulseIn(pin_sensor_echo, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("- Raw distance: ");
  Serial.println(distance);
  return distance;
}

// Average distance
int readDistanceAverage() {
  int index = 0;
  int sum_distance = 0;
  while(index < num_shots){
    sum_distance = sum_distance + readDistance();
    index = index + 1;
  }
  int end_distance = sum_distance/index;
  //Serial.println(index);
  //Serial.println(sum_distance);
  Serial.print("- Final distance: ");
  Serial.println(end_distance);
  return end_distance;
}

// Memory EEPROM

void writeIntIntoEEPROM(int address, int number)
{ 
  byte byte1 = number >> 8;
  byte byte2 = number & 0xFF;
  EEPROM.write(address, byte1);
  EEPROM.write(address + 1, byte2);
}

int readIntFromEEPROM(int address)
{
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}

// Memory position

bool stopMove () {
  byte buttonStateButton0 = digitalRead(pin_button_0);

  if (buttonStateButton0 == LOW) {
    Serial.println("- Button0 - Stop!");
    if (last_state_pin_button_0 == 1)
    {
     stop_on = true;
     return stop_on;
    }
    stop_on = false;
    return stop_on;
  }
}

void sensorWritePosition(int address)
{
  writeIntIntoEEPROM(address, readDistance());
}

int toPosition(int address)
{
  int memory_distance = readIntFromEEPROM(address);
  while(readDistance() < memory_distance){
    if (stopMove()) {
      break;
    }
    Up();
  }
  while(readDistance() > memory_distance){
    if (stopMove()) {
      break;
    }
    Down();
  }
  Serial.print("- [INFO] In position or stop!");
}

// Motors

void Up() {
  Serial.println("UP");
  motor1Up();
  motor2Up();
  delay(1000);
  motorStop();
}

void Down() {
  Serial.println("DOWN");
  motor1Down();
  motor2Down();
  delay(1000);
  motorStop();
}

void Up1() {
  Serial.println("UP_1");
  motor1Up();
  delay(1000);
  motorStop();
}

void Down1() {
  Serial.println("DOWN_1");
  motor1Down();
  delay(1000);
  motorStop();
}

void Up2() {
  Serial.println("UP_2");
  motor2Up();
  delay(1000);
  motorStop();
}

void Down2() {
  Serial.println("DOWN_2");
  motor2Down();
  delay(1000);
  motorStop();
}

// Led info
void LedOn() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}
void LedOff() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

// Motors
void motor1Up()
{
  digitalWrite(pint_int_1, HIGH);
  digitalWrite(pint_int_2, LOW);
}

void motor1Down()
{
  digitalWrite(pint_int_1, LOW);
  digitalWrite(pint_int_2, HIGH);
}

void motor2Up()
{
  digitalWrite(pint_int_4, HIGH);
  digitalWrite(pint_int_3, LOW);
}

void motor2Down()
{
  digitalWrite(pint_int_4, LOW);
  digitalWrite(pint_int_3, HIGH);
}

void motorStop()
{
  digitalWrite(pint_int_1, LOW);
  digitalWrite(pint_int_2, LOW);
  digitalWrite(pint_int_3, LOW);
  digitalWrite(pint_int_4, LOW);
}
