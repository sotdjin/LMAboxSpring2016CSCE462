#include <MaxMatrix.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <AFMotor.h>
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
int count = 0;
int val = 0;
bool mode_input = false;
bool intro_sign = false;
char c_ret_inner = 'q';

// PINS
int switch_pin = 11;
int arm_pin = 8;
int door_pin = 7;
int data = 6;    // DIN pin of MAX7219 module
int load = 13;    // CS pin of MAX7219 module
int clock = 10;  // CLK pin of MAX7219 module*/
int motor_direction = 12;
int motor_pwm = 3;
int motor_brake = 9;

int maxInUse = 2;  //how many MAX7219 are connected
MaxMatrix m(data, load, clock, maxInUse); // define Library
byte buffer[10];

void setup() {
  // Pin Setup
  pinMode(switch_pin, INPUT);
  arm_servo.attach(arm_pin);
  arm_servo.write(20);
  door_servo.attach(door_pin);
  door_servo.write(5);
  pinMode(APDS9960_INT, INPUT);
  pinMode(motor_direction, OUTPUT);
  pinMode(motor_brake, OUTPUT);
  
  // Random and Serial start
  Serial.begin(9600);
  randomSeed(analogRead(0));
  delay(50);
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
  if (intro_sign == false) {
    Serial.println("LMA BOX    ");
    char string3[] = "LMA BOX    ";  // Scrolling Text
    m.shiftLeft(false, true);
    printStringWithShift(string3, 75);
    intro_sign = true;
  }
  delay(1050);
  char gest;
  if (isr_flag == 1) {
    detachInterrupt(0);
    gest = handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
  Serial.println(gest);
  if (gest == 'u') {
    mode_input = true;
  }
  else {
    mode_input = false;
  }
  
  if (mode_input) {
    move_mode();
  }
  else {
    Serial.println("Choosing random!");
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
void move_mode() {
  Serial.println("MOVE MODE");
  char string3[] = " Run Away Mode ";  // Scrolling Text
  m.shiftLeft(false, true);
  printStringWithShift(string3, 75);
  bool return_main = false;
  while (return_main == false) {
    // MOVES AWAY UNTIL SWITCH IS HIT, THEN RETURNS
    c_ret_inner = 'q';
    int switch_state = digitalRead(switch_pin);
    if (isr_flag == 1) {
      detachInterrupt(0);
      handleGesture_2();
      isr_flag = 0;
      attachInterrupt(0, interruptRoutine, FALLING);
    }
    Serial.println(c_ret_inner);
    if (c_ret_inner != 'q' && switch_state == LOW) {
      digitalWrite(motor_direction, LOW);
      digitalWrite(motor_brake, LOW);
      analogWrite(motor_pwm, 150);
      count++;
      delay(600);
      digitalWrite(motor_brake, HIGH);
    }
    else if (switch_state == HIGH) {
      move_back_mode();
      rand_num = random(3);
      if (rand_num == 0) {
        normal_behavior();
      }
      else if (rand_num == 1) {
        sneak();
      }
      else {
        captain_crazy();
      }
      return_main = true;
    }
    else {
      digitalWrite(motor_brake, HIGH);
    }
  }
}
void move_back_mode() {
  Serial.println("GOING BACK");
  char string4[] = " Going Back! ";  // Scrolling Text
  m.shiftLeft(false, true);
  printStringWithShift(string4, 75);
  
  digitalWrite(motor_direction, HIGH);
  digitalWrite(motor_brake, LOW);
  analogWrite(motor_pwm, 150);
  delay(700 * count);
  digitalWrite(motor_brake, HIGH);
  count = 0;
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
  for (pos_arm = 20; pos_arm < 100; pos_arm += 3) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  // Closing Arm
  for (pos_arm = 100; pos_arm > 20; pos_arm -= 3) {
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
  for (pos_arm = 50; pos_arm < 100; pos_arm += 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_arm = 100; pos_arm > 20; pos_arm -= 4) {
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
void interruptRoutine() {
  isr_flag = 1;
}
char handleGesture() {
  char c_ret;
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:
        c_ret = 'u';
        break;
      case DIR_DOWN:
        c_ret = 'd';
        break;
      case DIR_LEFT:
        c_ret = 'l';
        break;
      case DIR_RIGHT:
        c_ret = 'r';
        break;
      case DIR_NEAR:
        c_ret = 'n';
        break;
      case DIR_FAR:
        c_ret = 'f';
        break;
      default:
        c_ret = 'q';
        break;
    }
  }
  return c_ret;
}
char handleGesture_2() {
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:
        c_ret_inner = 'u';
        break;
      case DIR_DOWN:
        c_ret_inner = 'd';
        break;
      case DIR_LEFT:
        c_ret_inner = 'l';
        break;
      case DIR_RIGHT:
        c_ret_inner = 'r';
        break;
      case DIR_NEAR:
        c_ret_inner = 'n';
        break;
      case DIR_FAR:
        c_ret_inner = 'f';
        break;
      default:
        c_ret_inner = 'q';
        break;
    }
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
