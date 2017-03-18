#include <NewPing.h>
/*
   Ici on définit la distance max de détection voulue sur les capteurs, laissez par défault si vous ne savez pas.
*/
#define NB_MESURES 2
#define SONAR_NUM 2
#define MAX_DISTANCE 0.50
#define VITESSE_SON 340.0

/*
   Nombre de milisecondes entre les captures successives, utilisées lors des perturbations dues à des pans inclinés,
   Vous pouvez régler cette valeur en calculant le temps de propagation sur la distance parcourue
   Plus il est long, plus vous prendrez de temps a faire les mesures
*/
const float delayMesureCapteur =  int(((2*MAX_DISTANCE)/(VITESSE_SON))*1000); 
//const float delayMesureCapteur = 500.0;

/*
   Le nombre de mesures effectuées, on fait la moyenne de ces mesures au final pour avoir une mesure plus précise
   Si on augmente, la mesure sera plus précise mais plus longue, et réciproquement
*/
/*
   Fonction à utiliser pour faire des mesures
   Modifiez a vos risques et périls
*/


float takeValue(NewPing sonar) 
{
  float sum = 0;
  float result;                               // Attention, si on ne fait pas la conversion directe, les valeurs sont tellement grandes qu'il faut utiliser un long
  int i = 0;
  while (i < NB_MESURES) 
  {             // Tant que l'on a pas le bon nombre de mesures valides
    result = sonar.ping();
    //Serial.println(delayMesureCapteur);
    if (result != 0) {                      // Si la valeur est 0, elle n'est pas valide
      sum += result / US_ROUNDTRIP_CM;          // On convertis en distance avec la valeur donnée dans la librairie                              // On compte une mesure valide de plus
    }
    i++;
    delay(delayMesureCapteur);          // Delai entre deux mesures pour éviter les interférences
  }
  return sum / NB_MESURES ;                 // On applique la moyenne
}

void echoCheck(NewPing parlisteSonar[],unsigned int parDistance[], int numCapteur)
{
  if (parlisteSonar[numCapteur].check_timer())
  {
    parDistance[numCapteur] = parlisteSonar[numCapteur].ping_result / US_ROUNDTRIP_CM;
  }
}

void unSeulCapteur(unsigned int parDistance[])
{
  for (int i = 0; i < SONAR_NUM; i++)
  {
    //Serial.print(i);
    //Serial.print("=");
    //Serial.print(distance[i]);
    //Serial.println("cm ");
  }
}

