#include <Servo.h>
#define ANGLE_MIN 45    
#define ANGLE_MAX 135
#define VITESSE 10

Servo myservo;
int pos = 0;

void setup() {
  myservo.attach(9);
  pinMode(2, OUTPUT);
 }
 
void loop() {
  for (pos = ANGLE_MIN; pos <= ANGLE_MAX; pos += 1) {
    myservo.write(pos);
    delay(VITESSE);
  }
  delay(100*VITESSE);
  digitalWrite(2, LOW);
    
  for(pos = ANGLE_MAX; pos >= ANGLE_MIN; pos -= 1) {
    myservo.write(pos);
    delay(VITESSE);
  }
  delay(100*VITESSE);
  digitalWrite(2, HIGH);
  }
