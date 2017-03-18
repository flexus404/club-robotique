#include <NewPing.h>
#include <digitalWriteFast.h>
#include <Stepper.h>
#include <SPI.h>


#define trigPin1 17
#define echoPin1 16
#define trigPin2 19
#define echoPin2 18

#define moteurGENA 37
#define moteurGENB 36
#define moteurGIN1 10
#define moteurGIN2 11
#define moteurGIN3 12
#define moteurGIN4 13

#define moteurDENA 34
#define moteurDENB 35
#define moteurDIN1 6
#define moteurDIN2 7
#define moteurDIN3 8
#define moteurDIN4 9

#define servo1 30
#define servo2 31

#define SDALCD 20
#define SCLLCD 21

#define MOSI 50
#define MISO 51

unsigned long pingTimer[SONAR_NUM];
unsigned int distance[SONAR_NUM];
int CapteurActuel = 0;

float uS;
int i = 0;
const int nbPasMoteur = 400;
const float taille_roue = 7.62;

//IL faut créer une liste de commande
int queue[128] = {0};

NewPing sonar[SONAR_NUM] =
{
  NewPing (trigPin1, echoPin1, MAX_DISTANCE),
  NewPing (trigPin2, echoPin2, MAX_DISTANCE)
};

Stepper moteurG(nbPasMoteur, moteurGIN1, moteurGIN2, moteurGIN3, moteurGIN4);
Stepper moteurD(nbPasMoteur, moteurDIN1, moteurDIN2, moteurDIN3, moteurDIN4);

Servo servos[NB_SERVO] =
{
  Servo (),
  Servo ()
};

void setup()
{
  Serial.begin (19200); //Pour pouvoir écrire sur le moniteur

  pingTimer[0] = millis() + 100;
  for (int i = 0; i < SONAR_NUM ; i++)
  {
    pingTimer[i] = pingTimer[i - 1] + delayMesureCapteur;
  }

  for (i = 0; i < NB_SERVO; i++)
  {
    servos[i].attach(servo1 + i);
    servos[i].write(0);
  }

  pinMode(moteurGENA, OUTPUT);
  pinMode(moteurGENB, OUTPUT);
  pinMode(moteurDENA, OUTPUT);
  pinMode(moteurDENB, OUTPUT);

  digitalWrite(moteurGENA, HIGH);
  digitalWrite(moteurGENB, HIGH);
  digitalWrite(moteurDENA, HIGH);
  digitalWrite(moteurDENB, HIGH);

  //déterminer les codes d'instruction
  queue[0] = 1;
  queue[1] = 2;
  queue[2] = 400;
  queue[3] = 1;
  queue[4] = 3;
  queue[5] = 1000;
  queue[6] = 0;

  Serial.println("Fin d'initialisation");
}

void endProg();

void loop()
{
  int i = 0;
  while(queue[i] != 0)
  {
    switch(queue[i])
    {
      case 1: //On lit les capteurs
        for (int i = 0; i < SONAR_NUM; i++)
        {
          if (millis() >= pingTimer[i])
          {
            /*pingTimer[i] += delayMesureCapteur * SONAR_NUM;
            sonar[CapteurActuel].timer_stop();
            CapteurActuel = i;
            distance[CapteurActuel] = 0;
            sonar[CapteurActuel].ping_timer(echoCheck)*/
          }
        }
      
        //uS = takeValue(sonarGauche); //On fait une mesure
        //On affiche le résultat
      
        //Serial.println(uS);
        //Serial.println(" cm");
        //Serial.print(delayMesureCapteur);
        break;
      case 2: //on avance d'un certain nombre de pas
        i++;
        avancerPas(moteurG, moteurD, queue[i], 100);
        break;
      case 3:
        i++; //on avance pendant un certain temps
        avancerTemps(moteurG, moteurD, queue[i], 100);
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        break;
      case 8:
        break;
      case 9:
        break;
      case 10:
        break;
      i++;
      Serial.print("instruction (");
      Serial.print(i+1);
      Serial.print(") : ");
      Serial.println(queue[i+1]);
    }
    
    
  }
  endProg();
}

void endProg()
{
  //funnyaction
  arret(moteurG, moteurD);
  while(1);
}

void echoCheck()
{
  if (sonar[CapteurActuel].check_timer())
  {
    distance[CapteurActuel] = sonar[CapteurActuel].ping_result / US_ROUNDTRIP_CM;
  }
}

void unSeulCapteur()
{
  for (int i = 0; i < SONAR_NUM; i++)
  {
    //Serial.print(i);
    //Serial.print("=");
    //Serial.print(distance[i]);
    //Serial.println("cm ");
  }
}

