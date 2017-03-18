#include <Stepper.h>

const int nb_pas_360 = 500; //Valeur à trouver de facon empirique

int currentVitesse = 0;
int currentDistanceG = 0;
int currentDistanceD = 0;

int avancerPas(Stepper parM1, Stepper parM2, int parPas, int parVitesseMax)
{
    /*parM1.setSpeed(parVitesseMax/2);
    parM2.setSpeed(parVitesseMax/2);
    parM1.step(parPas/4);
    parM2.step(parPas/4);
    parM1.setSpeed(parVitesseMax);
    parM2.setSpeed(parVitesseMax);
    currentVitesse = parVitesseMax;
    parM1.step(3*parPas/4);
    parM2.step(3*parPas/4);*/

    parM1.setSpeed(parVitesseMax);
    parM2.setSpeed(parVitesseMax);
    
    for (int i = 0; i < parPas; i++)
    {
      parM1.step(1);
      parM2.step(1);
    }
    
    currentDistanceG += parPas;
    currentDistanceD += parPas;
    
    return parPas;
}

int avancerTemps(Stepper parM1, Stepper parM2, unsigned long parTemps, int parVitesseMax)
{
  
  int i = 0;
  unsigned long debut = millis();
  Serial.println(parTemps);
  parM1.setSpeed(parVitesseMax);
  parM2.setSpeed(parVitesseMax);
  currentVitesse = parVitesseMax;
  
  while( debut + parTemps > millis() )
  {
    /*Serial.print(millis());
    Serial.print(" : ");
    Serial.println(parTemps+debut);*/
    parM1.step(2);
    parM2.step(2);
    i++;
  }

  currentDistanceG += i;
  currentDistanceD += i;
  return i;
}

int gauche(Stepper parM1, Stepper parM2, int parAngle)
{
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
}

void droite(Stepper parM1, Stepper parM2, int parAngle)
{
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
}

void arret(Stepper parM1, Stepper parM2)
{
  parM1.setSpeed(0);
  parM2.setSpeed(0);
  currentVitesse = 0;
}


