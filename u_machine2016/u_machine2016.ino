#include <uspeech.h>

#include <Servo.h>

Servo arm_servo, door_servo;

// VARIABLES
int pos_arm, pos_door, rand_num;
int switch_state = LOW;
int pir_state = LOW;
int val = 0;
int count = 0;
bool mode_input = false;
bool move_back = false;
bool door_open = false;

// PINS
int switch_pin = 2;
int arm_pin = 8;
int door_pin = 10;
int led_pin = 13;
int pir_pin = 4;
int motor_direction = 12;
int motor_brake = 9;
int motor_pwm = 3;
signal voice(A0);

void setup() {
  // Voice Coeff and Calibrate;
  voice.f_enabled = true;
  voice.minVolume = 2;
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
  //pinMode(led_pin, OUTPUT);

  // Random and Serial start
  randomSeed(analogRead(1));
  Serial.begin(9600);
}

void loop() {
  /*ALL THE WHILE CALL FUNCTION FOR LED DISPLAY, A GREETING AND MODE SWITCHES
    IF STATEMENT HERE FOR WAITING FOR MODE TYPE FROM SOUND SENSOR AND PARSE THROUGH uSPEECH {DEFAULT TO SWITCH MODE}*/
  //LED CODE HERE?
  char p = voice.getPhoneme();
  if (p != ' ') {
    if (p == 'o') {
      mode_input = true;
    }
    else {
      mode_input = false;
    }
  }
  else {
    /*STATEMENT FOR MICROPHONE USE FOR MODE TYPE USING MOTION SENSOR IN MOVING AWAY FROM USER
      WRITE CODE HERE FOR MOVING AWAY AND STUFF, WRITE CODE FOR MOVING THE BOX BACK INTO PLACE AFTERWARDS*/
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
  }
}
// 20 - 95 ARM ; 5 - 70 DOOR
void normal_behavior() {
  // LED DISPLAY?
  // Opening Door
  for (pos_door = 5; pos_door <= 70; pos_door += 1) {
    door_servo.write(pos_door);
    delay(15);
  }
  // Opening Arm
  for (pos_arm = 20; pos_arm <= 95; pos_arm += 1) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  // Closing Arm
  for (pos_arm = 95; pos_arm >= 20; pos_arm -= 1) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  // Closing Door
  for (pos_door = 70; pos_door >= 5; pos_door -= 1) {
    door_servo.write(pos_door);
    delay(15);
  }
}
// 20 - 95 ARM ; 5 - 70 DOOR
void sneak() {
  // LED DISPLAY?
  // Opening Door
  for (pos_door = 5; pos_door <= 50; pos_door += 1) {
    door_servo.write(pos_door);
    delay(30);
  }
  delay(2000);
  // Opening Arm
  for (pos_arm = 20; pos_arm <= 40; pos_arm += 1) {
    arm_servo.write(pos_arm);
    delay(30);
  }
  delay(500);
  for (pos_door = 50; pos_door <= 70; pos_door += 4) {
    door_servo.write(pos_door);
    delay(15);
  }
  delay(100);
  for (pos_arm = 40; pos_arm <= 70; pos_arm += 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(500);
  for (pos_arm = 70; pos_arm >= 50; pos_arm -= 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 50; pos_arm <= 70; pos_arm += 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 70; pos_arm >= 50; pos_arm -= 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 50; pos_arm <= 95; pos_arm += 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_arm = 95; pos_arm >= 20; pos_arm -= 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_door = 70; pos_door >= 5; pos_door -= 3) {
    door_servo.write(pos_door);
    delay(15);
  }
}
// 20 - 95 ARM ; 5 - 70 DOOR
void captain_crazy() {
  // LED DISPLAY?
  //Crazy Door Portion
  for (pos_door = 5; pos_door <= 60; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  for (pos_door = 60; pos_door >= 5; pos_door -= 5) {
    door_servo.write(pos_door);
    delay(15);
  }
  for (pos_door = 5; pos_door <= 50; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  for (pos_door = 50; pos_door >= 5; pos_door -= 15) {
    door_servo.write(pos_door);
    delay(15);
  }
  delay(500);
  for (pos_door = 5; pos_door <= 60; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  delay(500);
  for (pos_door = 60; pos_door >= 5; pos_door -= 5) {
    door_servo.write(pos_door);
    delay(15);
  }
  
  //"Functional" Part
  for (pos_door = 5; pos_door <= 70; pos_door += 8) {
    door_servo.write(pos_door);
    delay(15);
  }
  for (pos_arm = 20; pos_arm <= 95; pos_arm += 3) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_arm = 95; pos_arm >= 20; pos_arm -= 3) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  for (pos_door = 70; pos_door >= 5; pos_door -= 15) {
    door_servo.write(pos_door);
    delay(15);
  }
}
void open_door_slight() {
  for (pos_door = 5; pos_door <= 50; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  bool door_open = true;
}
void close_door_slight() {
  for (pos_door = 50; pos_door >= 5; pos_door += 3) {
    door_servo.write(pos_door);
    delay(15);
  }
  bool door_open = false;
}

