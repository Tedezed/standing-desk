// Motor
int PinIN1 = 2;
int PinIN2 = 3;
int PinIN3 = 4;
int PinIN4 = 5;

// Buttons
int PinButton1 = 9;
int PinButton2 = 10;
int PinButton3 = 11;
int PinButton4 = 12;

void setup() {
  Serial.begin(9600);
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(PinButton1, INPUT_PULLUP);
  pinMode(PinButton2, INPUT_PULLUP);
  pinMode(PinButton3, INPUT_PULLUP);
  pinMode(PinButton4, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("- [INFO] Init Arduino motor table control");
  LedOff();
}

void loop () {
  terminalControl();
  buttonControl();
}

void terminalControl () {
  if (Serial.available() > 0) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    Serial.println(s);

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

  byte buttonStateButton1 = digitalRead(PinButton1);
  byte buttonStateButton2 = digitalRead(PinButton2);
  byte buttonStateButton3 = digitalRead(PinButton3);
  byte buttonStateButton4 = digitalRead(PinButton4);
  
  if (buttonStateButton1 == LOW) {
    Serial.println("- Button1");
    Up();
  }
  if (buttonStateButton2 == LOW) {
    Serial.println("- Button2");
    Down();
  }
  if (buttonStateButton3 == LOW) {
    Serial.println("- Button3");
    Up2();
  }
  if (buttonStateButton4 == LOW) {
    Serial.println("- Button4");
    Down2();
  }
}


void Up() {
  Motor1Up();
  Motor2Up();
  delay(1000);
  MotorStop();
}

void Down() {
  Motor1Down();
  Motor2Down();
  delay(1000);
  MotorStop();
}

void Up1() {
  Motor1Up();
  delay(1000);
  MotorStop();
}

void Down1() {
  Motor1Down();
  delay(1000);
  MotorStop();
}

void Up2() {
  Motor2Up();
  delay(1000);
  MotorStop();
}

void Down2() {
  Motor2Down();
  delay(1000);
  MotorStop();
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
void Motor1Up()
{
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
}

void Motor1Down()
{
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
}

void Motor2Up()
{
  digitalWrite(PinIN4, HIGH);
  digitalWrite(PinIN3, LOW);
}

void Motor2Down()
{
  digitalWrite(PinIN4, LOW);
  digitalWrite(PinIN3, HIGH);
}

void MotorStop()
{
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, LOW);
}
