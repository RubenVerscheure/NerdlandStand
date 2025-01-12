// TEMPERATUUR
#include "max6675.h"

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

float Celsius = 0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

//RELAY (WARMTEELEMENT)
int relay_pin = 7;

//SWITCH
#include <ezButton.h>

ezButton limitSwitch(2);  


void setup() {
  Serial.begin(9600);

  //TEMPERATUUR
  Serial.println("MAX6675 test"); 
  delay(500);
  
  //RELAY
  pinMode(relay_pin,OUTPUT);
  digitalWrite(relay_pin,HIGH);

  //SWITCH
  limitSwitch.setDebounceTime(50);
}

void loop() {
  
  //TEMPERATUUR
  Celsius = thermocouple.readCelsius();

  Serial.print(Celsius);
  Serial.println(" C  ");

  //SWITCH
  limitSwitch.loop();

  //WARMTEELEMENT CONTROLE
  int state = limitSwitch.getState();
  if(state == HIGH)
    if (Celsius <=300) {
        digitalWrite(relay_pin,HIGH);
    }
      else { 
        digitalWrite(relay_pin,LOW);
    }
  else { 
        digitalWrite(relay_pin,LOW);
    }
  delay(1000);
}

