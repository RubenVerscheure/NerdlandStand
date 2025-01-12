
//TEMP
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float Object1 = 0;
float Ambient = 0;

//SCHERM
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//MOTOR
#include "AccelStepper.h"
#include "ezButton.h"

#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1

#define buttonPin 4
#define limitSwitchPin 7

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

ezButton button(buttonPin); 
ezButton limitSwitch(limitSwitchPin); 

bool motorRunning = false;   
bool motorDirection = true; 
bool knop = false;
bool limit = false;


void setup() {
  //TEMP
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);

  //SCHERM
  lcd.init();         
  lcd.backlight();

  Serial.begin(9600);
  while (!Serial);

  //TEMP
  Serial.println("Adafruit MLX90614 test");

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");

  //MOTOR
  button.setDebounceTime(50);        
  limitSwitch.setDebounceTime(50);  

  stepper.setMaxSpeed(1000);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(limitSwitchPin, INPUT_PULLUP);

}

void loop() {
  //MOTOR
  button.loop();
  limitSwitch.loop();

  //TEMP
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  
  Serial.println();

  float Ambient1 = mlx.readAmbientTempC();
  float Object1 = mlx.readObjectTempC();

  //SCHERM
  lcd.setCursor(1, 0);
  lcd.print("De temperatuur is   ");
  lcd.print(Object1,2);
  lcd.print(" C ");

  //TEMP & MOTOR
  if (Object1 >=30) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Start met");
        lcd.setCursor(1, 1);
        lcd.print("thermovormen!");

        digitalWrite(12,HIGH);
        digitalWrite(13,LOW);
        motorRunning = true;
        stepper.setSpeed(800);

        for (int i = 0; i < 20000; i++) { 
          stepper.runSpeed();
          delay(1); 
        }

        motorRunning = false; 
        
        
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Druk op knop");
    lcd.setCursor(1, 1);
    lcd.print("voor omkeren!");

    while (knop == false) {
        button.loop();
        delay(10);

        if (button.isPressed()) {
          knop = true;

          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Motor keert terug.");

          delay(10);

          motorDirection = false; 
          motorRunning = true; 
          stepper.setSpeed(-800);
        }

    }

    while (limit == false)  {
      stepper.runSpeed();

        limitSwitch.loop();

        if (limitSwitch.isPressed()) {

          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Motor is in positie");

          limit = true;
          motorRunning = false; 
          stepper.setSpeed(0);  
        }

    }
    motorDirection = true;
    knop = false;
    limit = false;

    }
      else { 
        digitalWrite(13,HIGH);
        digitalWrite(12,LOW);
    }

  delay(500);

}
