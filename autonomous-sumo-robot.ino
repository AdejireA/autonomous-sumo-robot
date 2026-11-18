// defines for IR
const int FL = A0;
const int FR = A1;
const int BL = A2;
const int BR = A3;

// defines for motor pins
const int in1 = 11;
const int in2 = 10;
const int in3 = 9;
const int in4 = 8;
const int ena = 5;
const int enb = 6;

// defines for ultrasonic
const int trigFront = 13;
const int echoFront = 12;
const int trigBack  = 7;
const int echoBack  = 4;

// threshold defines
const int blackThreshold = 300;
const int detectDistance = 15;


const int searchSpeed   = 120;
const int attackSpeed   = 200;
const int reverseSpeed  = 200;

bool escaping = false;
unsigned long escapeStart = 0;
const unsigned long escapeTime = 400;

// flags for ultrasonic
bool frontUS_enabled = true;
bool backUS_enabled  = true;
unsigned long usReenableTime = 0;
const unsigned long usDisableDuration = 1000;  // time to ignore ultrasonic after edge

void setup() {

  pinMode(FL, INPUT);
  pinMode(FR, INPUT);
  pinMode(BL, INPUT);
  pinMode(BR, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(trigBack, OUTPUT);
  pinMode(echoBack, INPUT);

  digitalWrite(trigFront, LOW);
  digitalWrite(trigBack, LOW);
}

void loop() {

  unsigned long now = millis();

  // re-enable the ultrasonic sensors
  if (now > usReenableTime) {
    frontUS_enabled = true;
    backUS_enabled  = true;
  }

  //check edge

  int fl = analogRead(FL);
  int fr = analogRead(FR);
  int bl = analogRead(BL);
  int br = analogRead(BR);

  bool frontBlack = (fl > blackThreshold) || (fr > blackThreshold);
  bool backBlack  = (bl > blackThreshold) || (br > blackThreshold);

  //enter escape mode
  if (escaping) {
    if (now - escapeStart >= escapeTime) {
      escaping = false;
      stopMotors();
    }
    return;
  }

  // detect front edge
  if (frontBlack) {

    frontUS_enabled = false;        
    usReenableTime = now + usDisableDuration;

    hardBrake();
    moveBackward(reverseSpeed);

    escapeStart = now;
    escaping = true;
    return;
  }

  // detect back edge
  if (backBlack) {

    backUS_enabled = false;         
    usReenableTime = now + usDisableDuration;

    hardBrake();
    moveForward(reverseSpeed);

    escapeStart = now;
    escaping = true;
    return;
  }

  // read distance from ultrasonic if black line is not detected
  long frontDist = -1;
  long backDist  = -1;

  if (frontUS_enabled) {
    frontDist = getDistance(trigFront, echoFront);
    delay(5);
  }

  if (backUS_enabled) {
    backDist = getDistance(trigBack, echoBack);
  }

  // attack section

  if (frontUS_enabled && frontDist > 0 && frontDist < detectDistance) {
    moveForward(attackSpeed);
    return;
  }

  if (backUS_enabled && backDist > 0 && backDist < detectDistance) {
    moveBackward(attackSpeed);
    return;
  }

  // return to search
  rotateRight(searchSpeed);
}

// motor direction

void moveForward(int spd) {
  analogWrite(ena, spd);
  analogWrite(enb, spd);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void moveBackward(int spd) {
  analogWrite(ena, spd);
  analogWrite(enb, spd);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void rotateRight(int spd) {
  analogWrite(ena, spd);
  analogWrite(enb, spd);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void stopMotors() {
  analogWrite(ena,0);
  analogWrite(enb,0);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void hardBrake() {
  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,HIGH);
  delay(30);
  stopMotors();
}

// ultrasonic section

long getDistance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000);

  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}