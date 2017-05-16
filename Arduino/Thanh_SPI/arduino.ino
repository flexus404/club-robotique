/* Arduino_Serial_Test.ino - SerialProtocol library - demo Copyright (c) 2014 NicoHood. All right reserved. 
   Program to test serial communication*/

// indicator led and button 
int           ledPin     = 9; 
int           buttonPin  = 8; 
unsigned long lastButton = 0;

void setup() 
{ 
  // Pin setup 
  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT); 
  digitalWrite(buttonPin, HIGH);
  Serial.begin(9600); 
  Serial.println("Hello World!"); 
  Serial.println("My name is Arduino Mega. Via Usb."); 
}

void loop() 
{ // send a ping with a button press 
  if(digitalRead(buttonPin)==LOW && millis()-lastButton>=200)
  { 
    Serial.println("Ping!"); // debounce 
    lastButton=millis(); 
  }
  
  // print received signal to debug output 
  if(Serial.available())
  { 
    digitalWrite(ledPin, HIGH); 
    char newChar=Serial1.read(); // print to debug Serial 
    Serial.write(newChar); 
  }
  
  digitalWrite(ledPin, LOW); 
}
