#include <NewPing.h>
#include <digitalWriteFast.h>


#define trigPin 12
#define echoPin 11
#define ENA 10
#define IN1 9
#define IN2 8
#define Pin1A 2    //  1ere sortie du codeur1
#define Pin1B 3    //  2e sortie du codeur1

#define Pin2A 21    //  1ere sortie du codeur2
#define Pin2B 20    //  2e sortie du codeur2

float uS;
NewPing sonarGauche(trigPin, echoPin, MAX_DISTANCE);

void setup() 
{
  Serial.begin (19200); //Pour pouvoir écrire sur le moniteur
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
    pinMode(Pin1A,INPUT);
  pinMode(Pin1B,INPUT); 
  pinMode(Pin2A,INPUT);
  pinMode(Pin2B,INPUT);  
  //attachInterrupt (0,routineEncodeur1,FALLING);   // interruption sur front descendant pin 2 => Voir fin programme
  //attachInterrupt (3,routineEncodeur2,FALLING);   // interruption sur front descendant pin 21 => Voir fin programme
  //ATTENTION: Il faut lire la doc de cette fonction pour l'utiliser, ce n'est pas intuitif (on peut consulter la doc sans internet)
  attachInterrupt (digitalPinToInterrupt(2),routineEncodeur1,FALLING);
  attachInterrupt (digitalPinToInterrupt(20),routineEncodeur2,FALLING);
  Serial.println("Fin d'initialisation");
}

void loop() 
{
    
  uS = takeValue(sonarGauche); //On fait une mesure
  //On affiche le résultat
  
  Serial.print(uS);
  Serial.println(" cm");
  
  digitalWrite(ENA, HIGH); 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(500);
  digitalWrite(ENA, HIGH); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(500);

  distance1=(diametre*PI)/(NBR_TICKS)*ticks1;
  distance2=(diametre*PI)/(NBR_TICKS)*ticks2;

  //Afficher la distance parcourue (facultatif)
  Serial.print("Distance 1 : ");
  Serial.print(distance1);
  Serial.print(" Distance 2 : ");
  Serial.println(distance2);
}
