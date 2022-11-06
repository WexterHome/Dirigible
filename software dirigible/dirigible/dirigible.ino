#include <Servo.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <nRF24L01.h>

const int pinCE = 2;
const int pinCSN = 4;
RF24 radio(pinCE, pinCSN);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int data[4] = {90, 0, 0, 0};

Servo left_servo;
Servo right_servo;

const int AIA = 3;
const int AIB = 5;
const int BIA = 7;
const int BIB = 6;

byte servos_angle = 90;
byte left_motor_speed = 0;
byte right_motor_speed = 0;



void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  delay(1000);

  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  pinMode(AIA, OUTPUT); 
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);

  left_servo.attach(9);
  right_servo.attach(10);
  left_servo.write(servos_angle);
  right_servo.write(servos_angle);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(radio.available()){
    radio.read(data, sizeof data);
    servos_angle = data[0];
    left_motor_speed = data[1];
    right_motor_speed = data[2];
    //Serial.println(left_motor_speed);
    left_servo.write(180-servos_angle);
    right_servo.write(servos_angle);
    cc_motors_handler();
  }
  delay(50);
}


//CONTROL MOTORES DE CORRIENTE CONTINUA
void cc_motors_handler(){
  if(data[3] == 0){
    analogWrite(AIA, 0);
    analogWrite(AIB, left_motor_speed);
    analogWrite(BIB, right_motor_speed);
  }

  else{
    analogWrite(AIA, right_motor_speed);
    analogWrite(AIB, 0);
    analogWrite(BIB, 0);
  }
}
 
