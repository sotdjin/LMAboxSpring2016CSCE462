#include <Servo.h>

Servo arm_servo, door_servo;
int pos_arm, pos_door, rand_num;
int switch_pin = 2;
int switch_state = LOW;
//int led_pin = 13;
//int pir_pin = 2;
//int pir_state = LOW;
//int motor_direction = 12;
//int motor_brake = 9;
//int motor_pwm = 3;
//int val = 0;
//int calibrationTime = 10;
//int currentValue;
//int maxValue;
//int minValue;
//unsigned long timer;
//int sampleSpan = 5;
//int volume;
//long unsigned int lowIn;
//long unsigned int pause = 20;
//signal voice(A0);
//boolean lockLow = true;
//boolean takeLowTime;

void setup() {  
  /*voice.f_enabled = true;
  voice.minVolume = 2;
  voice.fconstant = 400;
  voice.econstant = 1;
  voice.aconstant = 2;
  voice.vconstant = 3;
  voice.shconstant = 4;
  voice.calibrate();*/
  
  // put your setup code here, to run once:
  pinMode(switch_pin, INPUT);
  //pinMode(led_pin, OUTPUT);
  //pinMode(pir_pin, INPUT);
  //pinMode(motor_direction, OUTPUT);
  //pinMode(motor_brake, OUTPUT);
  arm_servo.attach(9);
  arm_servo.write(20);
  door_servo.attach(10);
  door_servo.write(5);
  randomSeed(analogRead(0));
  //digitalWrite(pir_pin, LOW);
  Serial.begin(9600);
  //resetValues();
}

void loop() {
  /*ALL THE WHILE CALL FUNCTION FOR LED DISPLAY, A GREETING AND MODE SWITCHES*/
  /*IF STATEMENT HERE FOR WAITING FOR MODE TYPE FROM SOUND SENSOR AND PARSE THROUGH uSPEECH {DEFAULT TO SWITCH MODE}*/
  //rand_num = random(4);
  rand_num = random(2);
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
    /*else if (rand_num == 2) {
      
    }
    else if (rand_num == 3) {
      
    }
    else {
      // Do nothing
    }*/
    
  }
  /*ELSE STATEMENT FOR MICROPHONE USE FOR MODE TYPE USING MOTION SENSOR IN MOVING AWAY FROM USER*/
  /*WRITE CODE HERE FOR MOVING AWAY AND STUFF, WRITE CODE FOR MOVING THE BOX BACK INTO PLACE AFTERWARDS*/
  
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
    digitalWrite(motor_direction, LOW);
    digitalWrite(motor_brake, LOW);
    //analogWrite(motor_pwm, 123);
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
  }*/
  
  /*Serial.println("forward full power");
  digitalWrite(motor_direction, LOW);
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
  delay(1000);*/
  
  /*val = digitalRead(pir_pin);
  if (val == HIGH) {
    digitalWrite(led_pin, HIGH);
    if (lockLow) {
      lockLow = false;            
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis()/1000);
      Serial.println(" sec"); 
      delay(50);
    }
    takeLowTime = true;
  }
  if (val == LOW) {
    digitalWrite(led_pin, LOW);
    if (takeLowTime) {
      lowIn = millis();
      takeLowTime = false;
    }
    if (!lockLow && millis() - lowIn > pause) {  
      //makes sure this block of code is only executed again after 
      //a new motion sequence has been detected
      lockLow = true;                        
      Serial.print("motion ended at ");      //output
      Serial.print((millis() - pause)/1000);
      Serial.println(" sec");
      delay(50);
    }
  }*/
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


