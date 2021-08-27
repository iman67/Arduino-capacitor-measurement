/*Thanks. Remember to visit my Youtube channel
  I've used a i2c LCD screen module. 
*/
//LCD config
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f,20,4);  //sometimes the adress is not 0x3f. Change to 0x27 if it dosn't work.

int ScalepF=8;
int ScalenF=13;

//High values//
#define analogPin      A0          
#define chargePin      5         
#define dischargePin   6       
#define resistorValue  10000.0F  //Remember, we've used a 10K resistor to charge the capacitor

unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                
float nanoFarads;

//Low values//
const int OUT_PIN = A2;
const int IN_PIN = A0;
const float IN_STRAY_CAP_TO_GND = 50.28; //We have to change te resistance in this configuration. The 10K and 220 resistors 
                                         //are changeing the values
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 30.0;  
const int MAX_ADC_VALUE = 1023;




void setup() {
  
  lcd.init();
  lcd.backlight();

  pinMode(ScalepF,INPUT);
  pinMode(ScalenF,INPUT);

  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  pinMode(chargePin, OUTPUT);     
   

}

void loop() {


//////////////////////////nF/////////////////////////////////////////

if(digitalRead(ScalenF))
{
  pinMode(OUT_PIN,OUTPUT);
  digitalWrite(OUT_PIN,LOW); //to make it GND
  pinMode(analogPin,INPUT); //This pin will read the voltage
  
  digitalWrite(chargePin, HIGH);  
  startTime = micros();
  while(analogRead(analogPin) < 648){       
  }

  elapsedTime= micros() - startTime;
  microFarads = ((float)elapsedTime / resistorValue) ;

     
  
  if (microFarads > 1){

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SCALE:  100nF-4F");
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
    lcd.print("SCALE:  100nF-4F");
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

//////////////////////////pF/////////////////////////////////////////

if(digitalRead(ScalepF))
{
    
    pinMode(chargePin,INPUT);
    pinMode(dischargePin,INPUT);//We give high impedance to the two pins. We don't use this pins


  
    pinMode(IN_PIN, INPUT);
    digitalWrite(OUT_PIN, HIGH);
    int val = analogRead(IN_PIN);
    digitalWrite(OUT_PIN, LOW);

    if (val < 976)
    {
      
      pinMode(IN_PIN, OUTPUT);

      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Scale:   1pF-1nF");
      lcd.setCursor(0,1);
      lcd.print(capacitance,3);
      lcd.setCursor(14,1);
      lcd.print("pF");
      delay(200);
    }
    
    else
    {
      pinMode(IN_PIN, OUTPUT);
      delay(1);
      pinMode(OUT_PIN, INPUT_PULLUP);
      unsigned long u1 = micros();
      unsigned long t;
      int digVal;

      do
      {
        digVal = digitalRead(OUT_PIN);
        unsigned long u2 = micros();
        t = u2 > u1 ? u2 - u1 : u1 - u2;
      } 
      
      while ((digVal < 1) && (t < 400000L));

      pinMode(OUT_PIN, INPUT);  
      val = analogRead(OUT_PIN);
      digitalWrite(IN_PIN, HIGH);
      int dischargeTime = (int)(t / 1000L) * 5;
      delay(dischargeTime);   
      pinMode(OUT_PIN, OUTPUT);  
      digitalWrite(OUT_PIN, LOW);
      digitalWrite(IN_PIN, LOW);

      float capacitance = -(float)t / R_PULLUP / log(1.0 - (float)val / (float)MAX_ADC_VALUE);

      lcd.setCursor(0,0);
      lcd.print("Scale:   1pF-1nF");
      
      if (capacitance > 1000.0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Scale:   1pF-1nF");
        lcd.setCursor(0,1);
        lcd.print(capacitance / 1000.0, 3);
        lcd.setCursor(14,1);
        lcd.print("uF ");
        delay(200);
        
      }
        
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Scale:   1pF-1nF");
        lcd.setCursor(0,1);
        lcd.print(capacitance,3);
        lcd.setCursor(14,1);
        lcd.print("nF");
        delay(200);
        
      }
  }
    while (micros() % 1000 != 0);
}
}
