#define ENA 10
#define IN1 9
#define IN2 8
int i = 0;
void setup() 
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  Serial.begin(19200);
}

void loop() 
{
  

  i++;
  Serial.println(i);
  digitalWrite(ENA, HIGH); 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(500);
  digitalWrite(ENA, HIGH); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(500);
}
