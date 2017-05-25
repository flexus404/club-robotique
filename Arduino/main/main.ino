///SYSTEME  A REALISER
/*
* module attente tirette
* module de fin de match
* test de la distance capteur (danger, true/false)
* calcul distance capteur de façon plus précise
* controle nouveau moteur pas à pas
* funnyaction
* rgb lcd
*/

///TRUCS A GARDER
/*
* tout capteurUltrason
* tout contrgit brancholeMoteur
*/

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
#define trigPin3 22
#define echoPin3 23

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

//Attente tirette//
#define tirette 24

///CONSTANTES///
//Moteurs pas à pas//
const int nbPasMoteur = 400;
const float taille_roue = 7.62;
const int vitesse_max_ent = 150;

//Ecran LCD RGB//
const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

//Capteurs ultrason
/*
   Ici on définit la distance max de détection voulue sur les capteurs, laissez par défault si vous ne savez pas.
*/
#define NB_MESURES 2
#define NB_SONAR 3
#define MAX_DISTANCE 0.50
#define VITESSE_SON 340.0
/*
   Nombre de milisecondes entre les captures successives, utilisées lors des perturbations dues à des pans inclinés,
   Vous pouvez régler cette valeur en calculant le temps de propagation sur la distance parcourue
   Plus il est long, plus vous prendrez de temps a faire les mesures
*/
const float delayMesureCapteur =  int(((2*MAX_DISTANCE)/(VITESSE_SON))*1000);
//const float delayMesureCapteur = 500.0;

//Controle moteur
const int nb_pas_360 = 500; //Valeur à trouver de facon empirique
static int currentVitesse = 0;
static int currentDistanceG = 0;
static int currentDistanceD = 0;

//Servo
#define NB_SERVO 2

///COMMANDES///

///VARIABLES GLOBALES///
//int queue[128] = {0}; //LA file d'instruction
//typedef int[2] commande;
QueueArray <int> queue;

unsigned int distance[NB_SONAR];
int CapteurActuel = 0;
float uS;
int i = 0;
bool process_it;
int pos = 0;
int buf[64] = {0};


///INITIALISATION OBJETS
//Le Timer principal
Timer t;
//Ecran LCD//
rgb_lcd lcd;
//Capteurs ultra-sons//
NewPing listeSonar[NB_SONAR] =
{
  NewPing(trigPin1, echoPin1, MAX_DISTANCE),
  NewPing(trigPin2, echoPin2, MAX_DISTANCE),
  NewPing(trigPin3, echoPin3, MAX_DISTANCE)
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
void endProg(); //fonction executée à la fin du programme
void funnyaction();
boolean echoCheck(int numCapteur);
float takeValue(int numCapteur);
void detectObstacle();
int avancerPas(long parPas, int parVitesseMax);
int avancerTemps(unsigned long parTemps, int parVitesseMax);
void gauche(int parAngle);
void droite(int parAngle);
void arret();
void afficherLCD(char msg[]);
int envoyer(char cmd[]);
/*
float takeValue(NewPing sonar);
void detectObstacle();
void echoCheck(NewPing parlisteSonar[],unsigned int parDistance[], int numCapteur);
void unSeulCapteur(unsigned int parDistance[]);

int avancerPas(AccelStepper parM1, AccelStepper parM2, long parPas, int parVitesseMax);
int avancerTemps(MultiStepper parM, unsigned long parTemps, int parVitesseMax);
void gauche(MultiStepper parM, int parAngle);
void droite(MultiStepper parM, int parAngle);
void arret(AccelStepper parM1, AccelStepper parM2);*/

///INITIALISATION PROGRAMME///
void setup()
{
  Serial.begin (19200); //Pour pouvoir écrire sur le moniteur

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

  /*
  Initialisation tirette
   */
   pinMode(tirette, INPUT);

   while(digitalRead(tirette)==HIGH)
   {
   }

   t.after(90000, endProg);
   Serial.println("Tirette done");

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
    //On lit les capteurs
      case 1:
        /*for (int i = 0; i < NB_SONAR; i++)
        {
          if (millis() >= pingTimer[i])
          {
            pingTimer[i] += delayMesureCapteur * NB_SONAR;
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
        //on avance d'un certain nombre de pas
      case 2:
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
        avancerPas(l, vitesse_max_ent);
        break;
      case 3:
        //i++; //on avance pendant un certain temps
        avancerTemps((queue.pop()), vitesse_max_ent);
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
      default:
        break;
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
  funnyaction();
  while(1);
}

void funnyaction()
{

    return;
}

///Fonctions capteurs ultrason
/*
   Le nombre de mesures effectuées, on fait la moyenne de ces mesures au final pour avoir une mesure plus précise
   Si on augmente, la mesure sera plus précise mais plus longue, et réciproquement
*/
/*
   Fonction à utiliser pour faire des mesures
   Modifiez a vos risques et périls
*/
float takeValue(int numCapteur)
{
  float sum = 0;
  float result;                               // Attention, si on ne fait pas la conversion directe, les valeurs sont tellement grandes qu'il faut utiliser un long
  int i = 0;
  while (i < NB_MESURES)
  {             // Tant que l'on a pas le bon nombre de mesures valides
    result = listeSonar[numCapteur].ping();
    //Serial.println(delayMesureCapteur);
    if (result != 0) {                      // Si la valeur est 0, elle n'est pas valide
      sum += result / US_ROUNDTRIP_CM;          // On convertis en distance avec la valeur donnée dans la librairie                              // On compte une mesure valide de plus
    }
    i++;
    delay(delayMesureCapteur);          // Delai entre deux mesures pour éviter les interférences
  }
  return sum / NB_MESURES ;                 // On applique la moyenne
}

//Fonction de callback
void detectObstacle()
{ //return la valeur du premier capteur qui detecte l'obstacle
    int i;
    bool hasObstacle = false;
    for(i = 0; i < NB_SONAR; i++) //on test tout les capteurs
    {
        if(hasObstacle = echoCheck(i)) //si l'on est face a un obstacle
        {
            arret(); //on arrete les moteurs
            while(echoCheck(i)) //on attend qu'il n'y est plus d'obstacle
            {
                delay(100);
            }
        }
        Serial.println(distance[i]);
        Serial.println(hasObstacle);
        moteurG.run(); //on redemarre les moteurs
        moteurD.run();
    }
    Serial.println("Tick");
}

boolean echoCheck(int numCapteur)
{
  if (listeSonar[numCapteur].check_timer())
  {
    distance[numCapteur] = listeSonar[numCapteur].ping_median(NB_MESURES) / US_ROUNDTRIP_CM;
    return true;
  }
  else
  {
      return false;
  }
}


///Fonctions controle moteur
int avancerPas(long parPas, int parVitesseMax)
{

    moteurG.setCurrentPosition(0);
    moteurD.setCurrentPosition(0);
    moteurG.moveTo(parPas);
    moteurD.moveTo(parPas);
    while (moteurG.distanceToGo() > 0 || moteurD.distanceToGo() > 0)
    {
        moteurG.setSpeed(parVitesseMax);
        moteurD.setSpeed(parVitesseMax);
        moteurG.runSpeedToPosition();
        moteurD.runSpeedToPosition();
    }

    //delay(10000);
}

int avancerTemps(unsigned long parTemps, int parVitesseMax)
{
    /*
    parM1.setSpeed(parVitesseMax);
    parM2.setSpeed(parVitesseMax);
    t.after(parTemps, arret(parM1,parM2));
    parM1.runSpeed();
    parM2.runSpeed();*/

    ///A   VERIFIER QUE LE CALLBACK STOP BIEN LA FONCTION


  unsigned long debut = millis();
  Serial.println(parTemps);
  //parM1.setSpeed(parVitesseMax);
  //parM2.setSpeed(parVitesseMax);
  //currentVitesse = parVitesseMax;
  long pos[2] = {100, -100};
  moteurs.moveTo(pos);
  moteurs.run();
  while( debut + parTemps > millis() )
  {
    //Serial.print(millis());
    //Serial.print(" : ");
    Serial.println(millis());
    moteurs.run();
  }

}

void gauche(int parAngle)
{
  /*
  int j = 0;
  int nbPas = (parAngle/360.0) * nb_pas_360;
  for(int i = 0; i < nbPas; i++)
  {
    parM1.step(1);
    parM2.step(-1);
    j++;
  }
  currentDistanceG += j;
  currentDistanceD += j;
  //return 0;
  */
  long pos[2] = {0};
  long var = (360.0/(long)nb_pas_360)*(long)parAngle;
  pos[0] = var;
  pos[1] = -var;
  moteurs.moveTo(pos);
}

void droite(int parAngle)
{
  /*
  int j = 0;
  int nbPas = (parAngle/360.0) * nb_pas_360;
  for(int i = 0; i < nbPas; i++)
  {
    parM1.step(-1);
    parM2.step(1);
    j++;
  }
  currentDistanceG += j;
  currentDistanceD += j;
  //return 0;
  */
  gauche(-parAngle);
}

void arret()
{
  /*
  parM1.setSpeed(0);
  parM2.setSpeed(0);
  currentVitesse = 0;
  */
  moteurG.stop();
  moteurD.stop();
}
