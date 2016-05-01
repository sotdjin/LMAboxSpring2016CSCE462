#include <uspeech.h>

#include <Servo.h>

Servo arm_servo, door_servo;

// VARIABLES
int pos_arm, pos_door, rand_num;
int switch_state = LOW;
int pir_state = LOW;
int val = 0;
bool booltest;

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
      booltest = true;
    }
    else {
      booltest = false;
    }
  }
  else {
    if (booltest) {
      rand_num = random(4);
      int switch_state = digitalRead(switch_pin);
      if (switch_state == LOW) {
        normal_behavior();
        if (rand_num == 0) {
          Serial.println("here0");
          normal_behavior();
        }
        else if (rand_num == 1) {
          Serial.println("here1");
          sneak();
        }
        else if (rand_num == 2) {
          captain_slow();
        }
        else if (rand_num == 3) {
          jezza();
        }
        else {
          // Do nothing
        }  
      }
    }
    /*ELSE STATEMENT FOR MICROPHONE USE FOR MODE TYPE USING MOTION SENSOR IN MOVING AWAY FROM USER
    WRITE CODE HERE FOR MOVING AWAY AND STUFF, WRITE CODE FOR MOVING THE BOX BACK INTO PLACE AFTERWARDS*/
    else {
      open_door_slight();
      val = digitalRead(pir_pin);
      if (val == HIGH) {
        digitalWrite(motor_direction, LOW);
        digitalWrite(motor_brake, LOW);
        analogWrite(motor_pwm, 123);
        delay(1000);
        digitalWrite(motor_brake, HIGH);
        if (pir_state == LOW) {
          pir_state = HIGH;
        }
      }
      else {
        digitalWrite(motor_brake, HIGH);
        if (pir_state == HIGH) {
          pir_state = LOW;
        }
      }
    }
  }
}
void normal_behavior() {
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
void sneak() {
  // Opening Door
  //70 full door open
  for (pos_door = 5; pos_door < 50; pos_door += 1) {
    door_servo.write(pos_door);
    delay(30);
  }
  delay(2000);
  // Opening Arm
  // 95 full arm
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
  for (pos_arm = 70; pos_arm >= 50; pos_arm -= 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 50; pos_arm < 70; pos_arm += 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 70; pos_arm >= 50; pos_arm -= 4) {
    arm_servo.write(pos_arm);
    delay(15);
  }
  delay(100);
  for (pos_arm = 50; pos_arm < 95; pos_arm += 4) {
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
void captain_slow() {
  
}
void jezza() {
  
}
void open_door_slight() {
  
}

