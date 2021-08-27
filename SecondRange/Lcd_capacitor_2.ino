/*Thanks. Remember to visit my Youtube channel
  If you don't whant to Serial print the valeus just delete the serial. print lines
  and leave just the LCD print ones.
  I've used a i2c LCD screen module. 
*/
//LCD config
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f,20,4);  //sometimes the adress is not 0x3f. Change to 0x27 if it dosn't work.

#define analogPin      0          
#define chargePin      13         
#define dischargePin   8        
#define resistorValue  10000.0F  //Remember, we've used a 10K resistor to charge the capacitor

unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                
float nanoFarads;

void setup(){
  pinMode(chargePin, OUTPUT);     
  digitalWrite(chargePin, LOW); 
  lcd.init();
  lcd.backlight();
}

void loop(){
  digitalWrite(chargePin, HIGH);  
  startTime = micros();
  while(analogRead(analogPin) < 648){       
  }

  elapsedTime= micros() - startTime;
  microFarads = ((float)elapsedTime / resistorValue) ;

     
  
  if (microFarads > 1){

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SCALE:  0.1uF-4F");
    lcd.setCursor(0,1);  
    lcd.print(microFarads);
    lcd.setCursor(14,1);        
    lcd.print("uF");   
    delay(500);    
  }

  else{
    nanoFarads = microFarads * 1000.0; 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SCALE:  0.1uF-4F");
    lcd.setCursor(0,1);       
    lcd.print(nanoFarads);  
    lcd.setCursor(14,1);       
    lcd.print("nF");          
    delay(500); 
  }

  
  digitalWrite(chargePin, LOW);            
  pinMode(dischargePin, OUTPUT);            
  digitalWrite(dischargePin, LOW);     //discharging the capacitor     
  while(analogRead(analogPin) > 0){         
  }//This while waits till the capaccitor is discharged

  pinMode(dischargePin, INPUT);      //this sets the pin to high impedance
  
  lcd.setCursor(0,0);
  lcd.print("DISCHARGING.....");
  lcd.setCursor(0,1);  
  
  
}
