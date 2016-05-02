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
int val = 0;
int count = 0;
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
signal voice(A1);

void setup() {
  // Voice Coeff and Calibrate;
  voice.f_enabled = true;
  voice.minVolume = 340;
  voice.fconstant = 400;
  voice.econstant = 1;
  voice.aconstant = 2;
  voice.vconstant = 3;
  voice.shconstant = 4;
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
  randomSeed(analogRead(0));
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
  /*char p = voice.getPhoneme();
  if (p != ' ') {
    if (p == 'o') {
      mode_input = true;
    }
    else {
      mode_input = false;
    }
  }
  else {
    if (mode_input) {
      char q = voice.getPhoneme();
      if (p != ' ') {
        if (p == 'v') {
          move_back = true;
        }
        else {
          move_back = false;
        }
      }
      else {
        if (move_back) {
          // LED DISPLAY?
          digitalWrite(motor_direction, HIGH);
          digitalWrite(motor_brake, LOW);
          analogWrite(motor_pwm, 150);
          delay(200 * count);
          digitalWrite(motor_brake, HIGH);
          move_back = false;
          count = 0;
        }
        else {
          // LED DISPLAY?
          if (!door_open) {
            open_door_slight();
            delay(15);
          }
          val = digitalRead(pir_pin);
          if (val == HIGH) {
            if (door_open) {
              close_door_slight();
              delay(15);
            }
            digitalWrite(motor_direction, LOW);
            digitalWrite(motor_brake, LOW);
            analogWrite(motor_pwm, 150);
            count++;
            delay(200);
            digitalWrite(motor_brake, HIGH);
          }
          else {
            digitalWrite(motor_brake, HIGH);
          }
        }
      }
    }
    else {
      rand_num = random(3);
      Serial.println(rand_num);
      int switch_state = digitalRead(switch_pin);
      if (switch_state == LOW) {
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
  }*/
  if (isr_flag == 1) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

void interruptRoutine() {
  isr_flag = 1;
}
void handleGesture() {
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
      case DIR_FAR:
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");
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
void open_door_slight() {
  for (pos_door = 5; pos_door < 50; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  bool door_open = true;
}
void close_door_slight() {
  for (pos_door = 50; pos_door > 5; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  bool door_open = false;
}

