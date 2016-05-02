#include <MaxMatrix.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <AFMotor.h>
#include <uspeech.h>
#include <Servo.h>
#include <avr/pgmspace.h>

Servo arm_servo, door_servo;

#define APDS9960_INT    2 // Needs to be an interrupt pin

PROGMEM const unsigned char CH[] = {
  3, 8, B00000000, B00000000, B00000000, B00000000, B00000000, // space
  1, 8, B01011111, B00000000, B00000000, B00000000, B00000000, // !
  3, 8, B00000011, B00000000, B00000011, B00000000, B00000000, // "
  5, 8, B00010100, B00111110, B00010100, B00111110, B00010100, // #
  4, 8, B00100100, B01101010, B00101011, B00010010, B00000000, // $
  5, 8, B01100011, B00010011, B00001000, B01100100, B01100011, // %
  5, 8, B00110110, B01001001, B01010110, B00100000, B01010000, // &
  1, 8, B00000011, B00000000, B00000000, B00000000, B00000000, // '
  3, 8, B00011100, B00100010, B01000001, B00000000, B00000000, // (
  3, 8, B01000001, B00100010, B00011100, B00000000, B00000000, // )
  5, 8, B00101000, B00011000, B00001110, B00011000, B00101000, // *
  5, 8, B00001000, B00001000, B00111110, B00001000, B00001000, // +
  2, 8, B10110000, B01110000, B00000000, B00000000, B00000000, // ,
  4, 8, B00001000, B00001000, B00001000, B00001000, B00000000, // -
  2, 8, B01100000, B01100000, B00000000, B00000000, B00000000, // .
  4, 8, B01100000, B00011000, B00000110, B00000001, B00000000, // /
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // 0
  3, 8, B01000010, B01111111, B01000000, B00000000, B00000000, // 1
  4, 8, B01100010, B01010001, B01001001, B01000110, B00000000, // 2
  4, 8, B00100010, B01000001, B01001001, B00110110, B00000000, // 3
  4, 8, B00011000, B00010100, B00010010, B01111111, B00000000, // 4
  4, 8, B00100111, B01000101, B01000101, B00111001, B00000000, // 5
  4, 8, B00111110, B01001001, B01001001, B00110000, B00000000, // 6
  4, 8, B01100001, B00010001, B00001001, B00000111, B00000000, // 7
  4, 8, B00110110, B01001001, B01001001, B00110110, B00000000, // 8
  4, 8, B00000110, B01001001, B01001001, B00111110, B00000000, // 9
  2, 8, B01010000, B00000000, B00000000, B00000000, B00000000, // :
  2, 8, B10000000, B01010000, B00000000, B00000000, B00000000, // ;
  3, 8, B00010000, B00101000, B01000100, B00000000, B00000000, // <
  3, 8, B00010100, B00010100, B00010100, B00000000, B00000000, // =
  3, 8, B01000100, B00101000, B00010000, B00000000, B00000000, // >
  4, 8, B00000010, B01011001, B00001001, B00000110, B00000000, // ?
  5, 8, B00111110, B01001001, B01010101, B01011101, B00001110, // @
  4, 8, B01111110, B00010001, B00010001, B01111110, B00000000, // A
  4, 8, B01111111, B01001001, B01001001, B00110110, B00000000, // B
  4, 8, B00111110, B01000001, B01000001, B00100010, B00000000, // C
  4, 8, B01111111, B01000001, B01000001, B00111110, B00000000, // D
  4, 8, B01111111, B01001001, B01001001, B01000001, B00000000, // E
  4, 8, B01111111, B00001001, B00001001, B00000001, B00000000, // F
  4, 8, B00111110, B01000001, B01001001, B01111010, B00000000, // G
  4, 8, B01111111, B00001000, B00001000, B01111111, B00000000, // H
  3, 8, B01000001, B01111111, B01000001, B00000000, B00000000, // I
  4, 8, B00110000, B01000000, B01000001, B00111111, B00000000, // J
  4, 8, B01111111, B00001000, B00010100, B01100011, B00000000, // K
  4, 8, B01111111, B01000000, B01000000, B01000000, B00000000, // L
  5, 8, B01111111, B00000010, B00001100, B00000010, B01111111, // M
  5, 8, B01111111, B00000100, B00001000, B00010000, B01111111, // N
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // O
  4, 8, B01111111, B00001001, B00001001, B00000110, B00000000, // P
  4, 8, B00111110, B01000001, B01000001, B10111110, B00000000, // Q
  4, 8, B01111111, B00001001, B00001001, B01110110, B00000000, // R
  4, 8, B01000110, B01001001, B01001001, B00110010, B00000000, // S
  5, 8, B00000001, B00000001, B01111111, B00000001, B00000001, // T
  4, 8, B00111111, B01000000, B01000000, B00111111, B00000000, // U
  5, 8, B00001111, B00110000, B01000000, B00110000, B00001111, // V
  5, 8, B00111111, B01000000, B00111000, B01000000, B00111111, // W
  5, 8, B01100011, B00010100, B00001000, B00010100, B01100011, // X
  5, 8, B00000111, B00001000, B01110000, B00001000, B00000111, // Y
  4, 8, B01100001, B01010001, B01001001, B01000111, B00000000, // Z
  2, 8, B01111111, B01000001, B00000000, B00000000, B00000000, // [
  4, 8, B00000001, B00000110, B00011000, B01100000, B00000000, // \ backslash
  2, 8, B01000001, B01111111, B00000000, B00000000, B00000000, // ]
  3, 8, B00000010, B00000001, B00000010, B00000000, B00000000, // hat
  4, 8, B01000000, B01000000, B01000000, B01000000, B00000000, // _
  2, 8, B00000001, B00000010, B00000000, B00000000, B00000000, // `
  4, 8, B00100000, B01010100, B01010100, B01111000, B00000000, // a
  4, 8, B01111111, B01000100, B01000100, B00111000, B00000000, // b
  4, 8, B00111000, B01000100, B01000100, B00101000, B00000000, // c
  4, 8, B00111000, B01000100, B01000100, B01111111, B00000000, // d
  4, 8, B00111000, B01010100, B01010100, B00011000, B00000000, // e
  3, 8, B00000100, B01111110, B00000101, B00000000, B00000000, // f
  4, 8, B10011000, B10100100, B10100100, B01111000, B00000000, // g
  4, 8, B01111111, B00000100, B00000100, B01111000, B00000000, // h
  3, 8, B01000100, B01111101, B01000000, B00000000, B00000000, // i
  4, 8, B01000000, B10000000, B10000100, B01111101, B00000000, // j
  4, 8, B01111111, B00010000, B00101000, B01000100, B00000000, // k
  3, 8, B01000001, B01111111, B01000000, B00000000, B00000000, // l
  5, 8, B01111100, B00000100, B01111100, B00000100, B01111000, // m
  4, 8, B01111100, B00000100, B00000100, B01111000, B00000000, // n
  4, 8, B00111000, B01000100, B01000100, B00111000, B00000000, // o
  4, 8, B11111100, B00100100, B00100100, B00011000, B00000000, // p
  4, 8, B00011000, B00100100, B00100100, B11111100, B00000000, // q
  4, 8, B01111100, B00001000, B00000100, B00000100, B00000000, // r
  4, 8, B01001000, B01010100, B01010100, B00100100, B00000000, // s
  3, 8, B00000100, B00111111, B01000100, B00000000, B00000000, // t
  4, 8, B00111100, B01000000, B01000000, B01111100, B00000000, // u
  5, 8, B00011100, B00100000, B01000000, B00100000, B00011100, // v
  5, 8, B00111100, B01000000, B00111100, B01000000, B00111100, // w
  5, 8, B01000100, B00101000, B00010000, B00101000, B01000100, // x
  4, 8, B10011100, B10100000, B10100000, B01111100, B00000000, // y
  3, 8, B01100100, B01010100, B01001100, B00000000, B00000000, // z
  3, 8, B00001000, B00110110, B01000001, B00000000, B00000000, // {
  1, 8, B01111111, B00000000, B00000000, B00000000, B00000000, // |
  3, 8, B01000001, B00110110, B00001000, B00000000, B00000000, // }
  4, 8, B00001000, B00000100, B00001000, B00000100, B00000000, // ~
};

// VARIABLES
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;
int pos_arm, pos_door;
long rand_num;
int switch_state;
int pir_state = LOW;
int count = 0;
int val = 0;
bool mode_input = false;
bool move_back = false;
bool door_open = false;
bool intro_done = false;

// PINS
int switch_pin = 11;
int arm_pin = 8;
int door_pin = 7;
int pir_pin = 4;
int data = 6;    // DIN pin of MAX7219 module
int load = 13;    // CS pin of MAX7219 module
int clock = 10;  // CLK pin of MAX7219 module*/
int motor_direction = 12;
int motor_pwm = 3;
int motor_brake = 9;
signal voice(A0);

int maxInUse = 2;  //how many MAX7219 are connected
MaxMatrix m(data, load, clock, maxInUse); // define Library
byte buffer[10];

void setup() {
  // Voice Coeff and Calibrate;
  voice.f_enabled = true;
  voice.minVolume = 9700;
  voice.fconstant = 300;
  voice.econstant = 3;
  voice.aconstant = 4;
  voice.vconstant = 2;
  voice.shconstant = 7;
  voice.calibrate();

  // Pin Setup
  pinMode(switch_pin, INPUT);
  arm_servo.attach(arm_pin);
  arm_servo.write(20);
  door_servo.attach(door_pin);
  door_servo.write(5);
  pinMode(pir_pin, INPUT);
  pinMode(motor_direction, OUTPUT);
  pinMode(motor_brake, OUTPUT);
  pinMode(APDS9960_INT, INPUT);
  
  // Random and Serial start
  Serial.begin(9600);
  randomSeed(analogRead(1));
  delay(10);
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - Gesture"));
  Serial.println(F("--------------------------------"));

  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } 
  else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } 
  else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }

  m.init();
  m.setIntensity(5);
}

void loop() {
  if (!intro_done) {
    introduction();
  }
  
  char p = voice.getPhoneme();
  Serial.println(p);
  if (p != ' ') {
    if (p == 'e') {
      mode_input = true;
    }
    else {
      mode_input = false;
    }
  }
  else {
    if (mode_input == true) {
      move_away_mode();
    }
    else {
      Serial.println("Switch_Mode");
      char string2[] = "Switch Mode   ";  // Scrolling Text
      delay(100);
      m.shiftLeft(false, true);
      printStringWithShift(string2, 100);
      
      rand_num = random(3);
      int switch_state = digitalRead(switch_pin);
      if (switch_state == HIGH) {
        if (rand_num == 0) {
          Serial.println("here0");
          normal_behavior();
        }
        else if (rand_num == 1) {
          Serial.println("here1");
          sneak();
        }
        else {
          Serial.println("here2");
          captain_crazy();
        }
      }
    }
  }
}

void introduction() {
  char string1[] = "LMA BOX   ";  // Scrolling Text
  delay(100);
  m.shiftLeft(false, true);
  printStringWithShift(string1, 100);
  intro_done = true;
}
void move_away_mode() {
  Serial.println("move_away_mode()");
  char string3[] = "Move Away   ";  // Scrolling Text
  delay(100);
  m.shiftLeft(false, true);
  printStringWithShift(string3, 100);
  
  bool return_main = false;
  int f_or_b;
  while (!return_main) {
    char g = voice.getPhoneme();
    if (g != ' ') {
      if (g == 's') {
        return_main = true;
      }
      else if (g == 'f') {
        f_or_b = 1;
        return_main = false;
      }
      else {
        f_or_b = 0;
        return_main = false;
      }
    }
    else {
      if (!return_main && f_or_b == 0) {
        if (isr_flag == 1) {
          detachInterrupt(0);
          handleGesture();
          isr_flag = 0;
          attachInterrupt(0, interruptRoutine, FALLING);
        }
        Serial.println(val);
        if (val == 1) {
          digitalWrite(motor_direction, LOW);
          digitalWrite(motor_brake, LOW);
          analogWrite(motor_pwm, 200);
          count++;
          delay(1500);
          //digitalWrite(motor_brake, HIGH);
          val = 0;
        }
        else {
          //digitalWrite(motor_brake, HIGH);
        }
      }
      else if (!return_main && f_or_b == 1) {
        move_back_mode();
      }
    }
  }
}
void move_back_mode() {
  Serial.println("move_back_mode()");
  char string4[] = "Move Back   ";  // Scrolling Text
  delay(100);
  m.shiftLeft(false, true);
  printStringWithShift(string4, 100);
  
  digitalWrite(motor_direction, HIGH);
  digitalWrite(motor_brake, LOW);
  analogWrite(motor_pwm, 150);
  delay(1500 * count);
  digitalWrite(motor_brake, HIGH);
  count = 0;
}

void interruptRoutine() {
  isr_flag = 1;
}
void handleGesture() {
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:
        Serial.println("UP");
        val = 1;
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        val = 1;
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        val = 1;
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        val = 1;
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        val = 1;
      case DIR_FAR:
        Serial.println("FAR");
        val = 1;
        break;
      default:
        Serial.println("NONE");
        val = 0;
        break;
    }
  }
}
// 20 - 95 ARM ; 5 - 70 DOOR
void normal_behavior() {
  // LED DISPLAY?
  // Opening Door
  for (pos_door = 5; pos_door < 70; pos_door += 1) {
    door_servo.write(pos_door);
    delay(15);
  }
  // Opening Arm
  for (pos_arm = 20; pos_arm < 95; pos_arm += 1) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  // Closing Arm
  for (pos_arm = 95; pos_arm > 20; pos_arm -= 1) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  // Closing Door
  for (pos_door = 70; pos_door > 5; pos_door -= 1) {
    door_servo.write(pos_door);
    delay(15);
  }
}
// 20 - 95 ARM ; 5 - 70 DOOR
void sneak() {
  // LED DISPLAY?
  // Opening Door
  for (pos_door = 5; pos_door < 50; pos_door += 1) {
    door_servo.write(pos_door);
    delay(30);
  }
  delay(2000);
  // Opening Arm
  for (pos_arm = 20; pos_arm < 40; pos_arm += 1) {
    arm_servo.write(pos_arm);
    delay(30);
  }
  delay(500);
  for (pos_door = 50; pos_door < 70; pos_door += 4) {
    door_servo.write(pos_door);
    delay(15);
  }
  delay(100);
  for (pos_arm = 40; pos_arm < 70; pos_arm += 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(500);
  for (pos_arm = 70; pos_arm > 50; pos_arm -= 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 50; pos_arm < 70; pos_arm += 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 70; pos_arm > 50; pos_arm -= 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 50; pos_arm < 95; pos_arm += 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_arm = 95; pos_arm > 20; pos_arm -= 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_door = 70; pos_door > 5; pos_door -= 3) {
    door_servo.write(pos_door);
    delay(15);
  }
}
// 20 - 95 ARM ; 5 - 70 DOOR
void captain_crazy() {
  // LED DISPLAY?
  //Crazy Door Portion
  for (pos_door = 5; pos_door < 60; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  for (pos_door = 60; pos_door > 5; pos_door -= 5) {
    door_servo.write(pos_door);
    delay(15);
  }
  for (pos_door = 5; pos_door < 50; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  for (pos_door = 50; pos_door > 5; pos_door -= 15) {
    door_servo.write(pos_door);
    delay(15);
  }
  delay(500);
  for (pos_door = 5; pos_door < 60; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  delay(500);
  for (pos_door = 60; pos_door > 5; pos_door -= 5) {
    door_servo.write(pos_door);
    delay(15);
  }
  
  //"Functional" Part
  for (pos_door = 5; pos_door < 70; pos_door += 8) {
    door_servo.write(pos_door);
    delay(15);
  }
  for (pos_arm = 20; pos_arm < 95; pos_arm += 3) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_arm = 95; pos_arm > 20; pos_arm -= 3) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_door = 70; pos_door > 5; pos_door -= 15) {
    door_servo.write(pos_door);
    delay(15);
  }
}
void printCharWithShift(char c, int shift_speed) {
  if (c < 32) return;
  c -= 32;
  memcpy_P(buffer, CH + 7*c, 7);
  m.writeSprite(maxInUse*8, 0, buffer);
  m.setColumn(maxInUse*8 + buffer[0], 0);
  
  for (int i=0; i<buffer[0]+1; i++) 
  {
    delay(shift_speed);
    m.shiftLeft(false, false);
  }
}
// Extract characters from Scrolling text
void printStringWithShift(char* s, int shift_speed){
  while (*s != 0){
    printCharWithShift(*s, shift_speed);
    s++;
  }
}
