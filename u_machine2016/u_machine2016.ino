#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <AFMotor.h>
#include <uspeech.h>
#include <Servo.h>

Servo arm_servo, door_servo;

#define APDS9960_INT    2 // Needs to be an interrupt pin

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

// PINS
int switch_pin = 11;
int arm_pin = 8;
int door_pin = 7;
int led_pin = 13;
int pir_pin = 4;
int motor_direction = 12;
int motor_brake = 9;
int motor_pwm = 3;
signal voice(A0);

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
  pinMode(led_pin, OUTPUT);

  // Random and Serial start
  randomSeed(analogRead(1));
  Serial.begin(9600);
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
}

void loop() {
  //LED CODE HERE?
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

void move_away_mode() {
  Serial.println("move_away_mode()");
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
        Serial.println("Here_move_away");
        // LED DISPLAY?
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
          analogWrite(motor_pwm, 150);
          count++;
          delay(200);
          digitalWrite(motor_brake, HIGH);
          val = 0;
        }
        else {
          digitalWrite(motor_brake, HIGH);
        }
      }
      else if (!return_main && f_or_b == 1) {
        move_back_mode();
      }
    }
  }
}

void move_back_mode() {
  Serial.println("Here_move_back");
  // LED DISPLAY?
  digitalWrite(motor_direction, HIGH);
  digitalWrite(motor_brake, LOW);
  analogWrite(motor_pwm, 150);
  delay(200 * count);
  digitalWrite(motor_brake, HIGH);
  move_back = false;
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
