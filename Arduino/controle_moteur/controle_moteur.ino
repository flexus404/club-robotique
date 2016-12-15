#define ENA 13
#define IN1 12
#define IN2 11
int i = 0;
void setup() 
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

void loop() 
{
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  i++;
  Serial.println(i);
  digitalWrite(13, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  delay(250);
}
