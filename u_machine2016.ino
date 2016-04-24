#include <uspeech.h>

#include <Servo.h>

Servo arm_servo, door_servo;
int pos_arm, pos_door, rand_num;
//int switch_pin = 2;
//int switch_state = LOW;
int led_pin = 13;
int pir_pin = 2;
int pir_state = LOW;
int motor_direction = 12;
int motor_brake = 9;
int motor_pwm = 3;
//int val = 0;
//signal voice(A0);
//boolean booltest = false;

void setup() {  
  /*voice.f_enabled = true;
  voice.minVolume = 1500;
  voice.fconstant = 400;
  voice.econstant = 1;
  voice.aconstant = 2;
  voice.vconstant = 3;
  voice.shconstant = 4;
  voice.calibrate();*/
  
  // put your setup code here, to run once:
  //pinMode(switch_pin, INPUT);
  //pinMode(led_pin, OUTPUT);
  pinMode(pir_pin, INPUT);
  pinMode(motor_direction, OUTPUT);
  pinMode(motor_brake, OUTPUT);
  //arm_servo.attach(9);
  //arm_servo.write(20);
  //door_servo.attach(10);
  //door_servo.write(5);
  //randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop() {
  
  /*rand_num = random(7);
  int switch_state = digitalRead(switch_pin);
  if (switch_state == LOW) {
    test_function();
  }*/
  
  /*char p = voice.getPhoneme();
  if (p != ' ') {
    if (p == 'f') {
      booltest = true;
    }
    else {
      booltest = false;
    }
  }
  else {
    if (booltest) {
      digitalWrite(led_pin, LOW);
    }
    else {
      digitalWrite(led_pin, HIGH);
    }
  }*/
  
  /*val = digitalRead(pir_pin);
  if (val == HIGH) {
    digitalWrite(motor_direction, HIGH);
    digitalWrite(motor_brake, LOW);
    analogWrite(motor_pwm, 255);
    if (pir_state == LOW) {
      pir_state = HIGH;
    }
  }
  else {
    digitalWrite(motor_direction, LOW);
    digitalWrite(motor_brake, LOW);
    analogWrite(motor_pwm, 255);
    if (pir_state == HIGH) {
      pir_state = LOW;
    }
  }*/
  Serial.println("forward full power");
  digitalWrite(motor_direction, HIGH);
  digitalWrite(motor_brake, LOW);
  analogWrite(motor_pwm, 255);

  delay(3000);
  Serial.println("brake");
  digitalWrite(motor_brake, HIGH);
  delay(1000);

  Serial.println("backward half power");
  digitalWrite(motor_direction, LOW);
  digitalWrite(motor_brake, LOW);
  analogWrite(motor_pwm, 123);

  delay(3000);
  Serial.println("brake");
  digitalWrite(motor_brake, HIGH);
  delay(1000);
}

void test_function() {
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
