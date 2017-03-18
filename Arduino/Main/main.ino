///INCLUDES///
#include <NewPing.h>
//#include <digitalWriteFast.h>
#include <Stepper.h>
#include <SPI.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Timer.h"

///PORTS CONNECTES///
//Ultrasons//
#define trigPin1 17
#define echoPin1 16
#define trigPin2 19
#define echoPin2 18

//Moteurs pas à pas//
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

//Servomoteurs//
#define servo1 30
#define servo2 31

//Ecran LCD RGB//
#define SDALCD 20
#define SCLLCD 21

//Com SPI Rasp//
#define MOSI 50
#define MISO 51

///CONSTANTES///
//Moteurs pas à pas//
const int nbPasMoteur = 400;
const float taille_roue = 7.62;
//Ecran LCD RGB//
const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

///COMMANDES///

///VARIABLES GLOBALES///
int queue[128] = {0}; //LA file d'instruction

unsigned long pingTimer[SONAR_NUM];
unsigned int distance[SONAR_NUM];
int CapteurActuel = 0;
float uS;
int i = 0;

///INITIALISATION OBJETS
//Ecran LCD//
rgb_lcd lcd;
//Capteurs ultra-sons//
NewPing listeSonar[SONAR_NUM] =
{
  NewPing(trigPin1, echoPin1, MAX_DISTANCE),
  NewPing(trigPin2, echoPin2, MAX_DISTANCE)
};
//Moteurs pas à pas//
Stepper moteurG(nbPasMoteur, moteurGIN1, moteurGIN2, moteurGIN3, moteurGIN4);
Stepper moteurD(nbPasMoteur, moteurDIN1, moteurDIN2, moteurDIN3, moteurDIN4);
//Servomoteurs//
Servo servos[NB_SERVO] =
{
  Servo (),
  Servo ()
};

///PROTOTYPES///
void endProg();
void echoCheck(NewPing parlisteSonar[],unsigned int parDistance[], int numCapteur);
void unSeulCapteur(unsigned int parDistance[]);
float takeValue(NewPing listeSonar);
int avancerPas(Stepper parM1, Stepper parM2, int parPas, int parVitesseMax);
int avancerTemps(Stepper parM1, Stepper parM2, unsigned long parTemps, int parVitesseMax);
int gauche(Stepper parM1, Stepper parM2, int parAngle);
void droite(Stepper parM1, Stepper parM2, int parAngle);
void arret(Stepper parM1, Stepper parM2);
void afficherLCD(char msg[]);
int envoyer(char cmd[]);

///INITIALISATION PROGRAMME///
void setup()
{
  Serial.begin (19200); //Pour pouvoir écrire sur le moniteur
  
  pingTimer[0] = millis() + 100;
  for (int i = 0; i < SONAR_NUM ; i++)
  {
    pingTimer[i] = pingTimer[i - 1] + delayMesureCapteur;
  }
  Serial.println("Ping done");

  for (i = 0; i < NB_SERVO; i++)
  {
    servos[i].attach(servo1 + i);
    servos[i].write(0);
  }
  Serial.println("Servo done");

  pinMode(moteurGENA, OUTPUT);
  pinMode(moteurGENB, OUTPUT);
  pinMode(moteurDENA, OUTPUT);
  pinMode(moteurDENB, OUTPUT);

  digitalWrite(moteurGENA, HIGH);
  digitalWrite(moteurGENB, HIGH);
  digitalWrite(moteurDENA, HIGH);
  digitalWrite(moteurDENB, HIGH);
  Serial.println("Stepper done");

  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  pos = 0;
  process_it = false;
  SPI.attachInterrupt();
  Serial.println("SPI done");
  /*
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  Serial.println("LCD done");
*/
  //déterminer les codes d'instruction
  queue[0] = 1;
  queue[1] = 2;
  queue[2] = 0;
  queue[3] = 1;
  queue[4] = 3;
  queue[5] = 5000;
  queue[6] = 0;


  Serial.println("Fin d'initialisation");
}


///BOUCLE PRINCIPALE///
void loop()
{
  //afficherLCD('Club Robot');
  //lcd.print("Club Robot");
  int i = 0;
  while(queue[i] != 0)
  {
    //Serial.println(queue[i]);
    switch(queue[i])
    {
      case 1: //On lit les capteurs
        /*for (int i = 0; i < SONAR_NUM; i++)
        {
          if (millis() >= pingTimer[i])
          {
            pingTimer[i] += delayMesureCapteur * SONAR_NUM;
            listeSonar[CapteurActuel].timer_stop();
            CapteurActuel = i;
            distance[CapteurActuel] = 0;
            listeSonar[CapteurActuel].ping_timer(echoCheck)
            listeSonar.ping_median();
          }
        }*/
      
        //uS = takeValue(listeSonarGauche); //On fait une mesure
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
        i++;
        if (process_it)
        {
          buf [pos] = 0;  
          //Serial.println (buf);
          pos = 0;
          process_it = false;
        } 
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
      //default:
      //break;
      
      
    }
    i++;
    Serial.println("");
    Serial.print("instruction (");
    Serial.print(i);
    Serial.print(") : ");
    Serial.println(queue[i]);
  }
  endProg();
}

///FONCTION DE FIN DE PROGRAMME
void endProg()
{
  Serial.println("Fin du programme");
  //funnyaction
  arret(moteurG, moteurD);
  while(1);
}


