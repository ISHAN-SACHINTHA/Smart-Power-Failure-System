
#include <LCD5110_Basic.h>

#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(0,1);
 
#include <String.h>


extern unsigned char SmallFont[];
String result ="";


char msg[250];
int i=0;
boolean started=false;

LCD5110 lcd(7,6,5,4,3);
int backlight=9;



int sensorpin = 0; //sensor attach to A0 pin
int sensorValue; // variable to read the value from the analog pin

boolean displayenable = false ;  
unsigned long previousMillis = 0;   
const long interval = 1000;     // interval at which to enable display (milliseconds)
int pushButton = 8;

void setup()
{
  analogWrite(backlight,155);
  lcd.InitLCD();
  lcd.setFont(SmallFont);
  lcd.print("Connecting",CENTER,0);

  pinMode(8,INPUT_PULLUP);
  pinMode(pushButton, INPUT);


  gprsSerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
  
 
  delay(1000);
}
 
void loop()
{

  int buttonstate = digitalRead(pushButton);

  unsigned long currentMillis = millis();

if ( buttonstate == HIGH){

  lcd.disableSleep();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

     lcd.enableSleep();
  }
}

  
      sensorValue = analogRead(sensorpin); 
      sensorValue = map(sensorValue, 0, 1023, 0, 230); // scale it to use it with the voltage (value between 0 and 230)
      delay(1000);   
         
      Serial.print("Voltage = ");
      Serial.println(sensorValue);
      
      
   
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  Serial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  Serial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  Serial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  Serial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  Serial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  Serial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  Serial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"dialogbb\"");//start task and setting the APN,
  Serial.println("AT+CSTT=\"dialogbb\"");
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  Serial.println("AT+CIICR");
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  Serial.println("AT+CIFSR");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  Serial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(6000);

  lcd.clrScr();
  lcd.print("Connected!",CENTER,0);
  Serial.println("Connected!");
  lcd.clrScr();
  lcd.print("Voltage",CENTER,0);
  lcd.print(sensorValue+"V",CENTER,16);
  
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  Serial.println("AT+CIPSEND");
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=O13AOCHYYNU2LQ19&field1=" + String(sensorValue);
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
  

} 
void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
  
}
