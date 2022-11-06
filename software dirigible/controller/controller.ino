#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

//Módulo RF
const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);
const uint64_t pipe = 0xE8E8F0F0E1LL;

/*
 * Primer dato: grados de cada servo.
 * Segundo dato: Velocidad motor izquierda
 * Tercer dato: Velocidad motor derecha
 * Cuarto dato: Hacia delante (0) o hacia atrás (1)
 */
int data[4]= {90, 0, 0, 0};
unsigned long lastTime = 0;
int threshold = 100;

//Controles
const int LEFT_JOYSTICK_VRY = A3;
const int RIGHT_JOYSTICK_VRX = A2;
const int GO_DOWN_L1 = 3;
const int GO_UP_R1 = 2;
const int GO_STRAIGHT_ARROW_UP = 6;

void setup(void){ 
  delay(1000);
  Serial.begin(9600);
  delay(1000);

  pinMode(GO_DOWN_L1, INPUT_PULLUP);
  pinMode(GO_UP_R1, INPUT_PULLUP);
  pinMode(GO_STRAIGHT_ARROW_UP, INPUT_PULLUP);
  
  radio.begin();
  radio.openWritingPipe(pipe);
}
 
void loop(void){
  if((millis()-lastTime) > threshold){
    lastTime = millis();

    if(!digitalRead(GO_DOWN_L1))
      data[0] = 180;
    else if(!digitalRead(GO_UP_R1))
      data[0] = 0;
    else if(!digitalRead(GO_STRAIGHT_ARROW_UP))
      data[0] = 90;

    controlDirigible();
    Serial.println(data[2]);
    radio.write(data, sizeof data);
  }
}

void controlDirigible(){
  int right_vrx_lecture = analogRead(RIGHT_JOYSTICK_VRX);
  int left_vry_lecture = analogRead(LEFT_JOYSTICK_VRY);

  if(right_vrx_lecture > 525 && right_vrx_lecture <= 1023){
    data[1] = 0;
    data[2] = map(right_vrx_lecture, 525, 1023, 100, 255);
  }

  else if(right_vrx_lecture < 500 && right_vrx_lecture >= 0){
    data[1] = map(right_vrx_lecture, 500, 0, 100, 255);
    data[2] = 0;
  }

  else if(left_vry_lecture > 525 && left_vry_lecture <=1023){
    data[1] = map(left_vry_lecture, 512, 1023, 100, 255);
    data[2] = data[1];
    data[3] = 0;  //Hacia delante
  }

  else if(left_vry_lecture < 500 && left_vry_lecture >= 0){
    data[1] = map(left_vry_lecture, 500, 0, 100, 255);
    data[2] = data[1];
    data[3] = 1;  //Hacia atrás
  }

  else{
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
  }
}
