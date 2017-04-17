#include <AccelStepper.h>
#include <MultiStepper.h>
#include "Timer.h"

const int nb_pas_360 = 500; //Valeur à trouver de facon empirique

static int currentVitesse = 0;
static int currentDistanceG = 0;
static int currentDistanceD = 0;

int avancerPas(MultiStepper parM, long parPas, int parVitesseMax)
{
  /*
    parM1.setSpeed(parVitesseMax);
    parM2.setSpeed(parVitesseMax);

    for (int i = 0; i < parPas; i++)
    {
      parM1.step(1);
      parM2.step(-1);
    }

    currentDistanceG += parPas;
    currentDistanceD += parPas;

    return parPas;*/
    parM.moveTo(parPas);
}

int avancerTemps(MultiStepper parM, unsigned long parTemps, int parVitesseMax)
{
    /*
    parM1.setSpeed(parVitesseMax);
    parM2.setSpeed(parVitesseMax);
    t.after(parTemps, arret(parM1,parM2));
    parM1.runSpeed();
    parM2.runSpeed();*/

    ///A   VERIFIER QUE LE CALLBACK STOP BIEN LA FONCTION


  unsigned long debut = millis();
  //Serial.println(parTemps);
  //parM1.setSpeed(parVitesseMax);
  //parM2.setSpeed(parVitesseMax);
  //currentVitesse = parVitesseMax;

  while( debut + parTemps > millis() )
  {
    //Serial.print(millis());
    //Serial.print(" : ");
    //Serial.println(parTemps+debut);
    parM.moveTo(10);
  }

}

void gauche(MultiStepper parM, int parAngle)
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
  parM.moveTo(pos);
}

void droite(MultiStepper parM, int parAngle)
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
  gauche(parM, -parAngle);
}

void arret(AccelStepper parM1, AccelStepper parM2)
{
  /*
  parM1.setSpeed(0);
  parM2.setSpeed(0);
  currentVitesse = 0;
  */
  parM1.stop();
  parM2.stop();
}
