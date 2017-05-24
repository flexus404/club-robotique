///INCLUDES///
#include <NewPing.h>
//#include <digitalWriteFast.h>
#include <Stepper.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include "rgb_lcd.h"
#include "Timer.h"
#include <QueueArray.h>

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
const int vitesse_max_ent = 150;
//Ecran LCD RGB//
const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

///COMMANDES///

///VARIABLES GLOBALES///
//int queue[128] = {0}; //LA file d'instruction
//typedef int[2] commande;
QueueArray <int> queue;

unsigned long pingTimer[SONAR_NUM];
unsigned int distance[SONAR_NUM];
int CapteurActuel = 0;
float uS;
int i = 0;

///INITIALISATION OBJETS
//Le Timer principal
Timer t;
//Ecran LCD//
rgb_lcd lcd;
//Capteurs ultra-sons//
NewPing listeSonar[SONAR_NUM] =
{
  NewPing(trigPin1, echoPin1, MAX_DISTANCE),
  NewPing(trigPin2, echoPin2, MAX_DISTANCE)
};
//Moteurs pas à pas//
/*Stepper moteurG(nbPasMoteur, moteurGIN1, moteurGIN2, moteurGIN3, moteurGIN4);
Stepper moteurD(nbPasMoteur, moteurDIN1, moteurDIN2, moteurDIN3, moteurDIN4);*/
AccelStepper moteurG(AccelStepper::FULL4WIRE, moteurGIN1, moteurGIN2, moteurGIN3, moteurGIN4);
AccelStepper moteurD(AccelStepper::FULL4WIRE, moteurDIN1, moteurDIN2, moteurDIN3, moteurDIN4);

MultiStepper moteurs;

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
void detectObstacle();
int avancerPas(AccelStepper parM1, AccelStepper parM2, long parPas, int parVitesseMax);
int avancerTemps(MultiStepper parM, unsigned long parTemps, int parVitesseMax);
void gauche(MultiStepper parM, int parAngle);
void droite(MultiStepper parM, int parAngle);
void arret(AccelStepper parM1, AccelStepper parM2);
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
/*
  pinMode(moteurGENA, OUTPUT);
  pinMode(moteurGENB, OUTPUT);
  pinMode(moteurDENA, OUTPUT);
  pinMode(moteurDENB, OUTPUT);

  digitalWrite(moteurGENA, HIGH);
  digitalWrite(moteurGENB, HIGH);
  digitalWrite(moteurDENA, HIGH);
  digitalWrite(moteurDENB, HIGH);
  */

  moteurG.setMaxSpeed(400);
  moteurD.setMaxSpeed(400);
  moteurG.setSpeed(80);
  moteurD.setSpeed(80);
  /*
  moteurG.setCurrentPosition(0);
  moteurD.setCurrentPosition(0);*/

  moteurs.addStepper(moteurG);
  moteurs.addStepper(moteurD);



  Serial.println("Stepper done");

  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  pos = 0;
  process_it = false;
  SPI.attachInterrupt();
  Serial.println("SPI done");

  /*t.every(100, detectObstacle);*/ //Timer.h
  //https://brainy-bits.com/blogs/tutorials/speed-sensor-with-arduino
  Serial.println("Timer done");

  /*
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  Serial.println("LCD done");
*/
  //déterminer les codes d'instruction
  /*queue[0] = 1;
  queue[1] = 2;
  queue[2] = 500;
  queue[3] = 1;
  queue[4] = 3;
  queue[5] = 1000;
  queue[]
  queue[6] = 0;*/
  queue.push(1);
  queue.push(2);
  queue.push(500);



  Serial.println("Fin d'initialisation");
}


///BOUCLE PRINCIPALE///
void loop()
{
  //moteurG.runSpeed();
  t.update();
  //afficherLCD('Club Robot');
  //lcd.print("Club Robot");
  int i = 0;
  int cmd = 0;
  int l;
  long position[2];

  //while(queue[i] != 0)
  while(!queue.isEmpty())
  {
    //Serial.println(queue[i]);
    cmd = queue.pop();
    switch(cmd)
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
        //i++;
        /*long positions[2];
        positions[0] = 1000;
        positions[1] = -1000;
        moteurs.moveTo(positions);
        moteurs.runSpeedToPosition();*/
        /*moteurG.setSpeed(vitesse_max_ent);
        moteurD.setSpeed(vitesse_max_ent);*/
        Serial.println("Case 2");
        l = queue.pop();
        Serial.println(l);
        avancerPas(moteurG,moteurD, l, vitesse_max_ent);
        break;
      case 3:
        //i++; //on avance pendant un certain temps
        avancerTemps(moteurs, (queue.pop()), vitesse_max_ent);
        break;
      case 4:
        //i++;
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
    Serial.println(cmd);


  }
  endProg();
}

///FONCTION DE FIN DE PROGRAMME
void endProg()
{
  Serial.println("Fin du programme");
  //funnyaction
  //arret(moteurG, moteurD);
  //moteurG.setSpeed(0);
  //moteurD.setSpeed(0);
  moteurG.disableOutputs();
  moteurD.disableOutputs();
  while(1);
}
