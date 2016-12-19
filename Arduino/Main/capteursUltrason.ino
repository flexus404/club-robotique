
#include <NewPing.h>
#define NB_MESURES 10
  /*
   * Ici on définit la distance max de détection voulue sur les capteurs, laissez par défault si vous ne savez pas.
   */
#define MAX_DISTANCE 200

   /*
    * Nombre de milisecondes entre les captures successives, utilisées lors des perturbations dues à des pans inclinés,
    * Vous pouvez régler cette valeur en calculant le temps de propagation sur la distance parcourue
    * Plus il est long, plus vous prendrez de temps a faire les mesures
    */
#define DELAI_MESURES_CAPTEURS 1

  /*
   * Le nombre de mesures effectuées, on fait la moyenne de ces mesures au final pour avoir une mesure plus précise
   * Si on augmente, la mesure sera plus précise mais plus longue, et réciproquement
   */
/*
 * Fonction à utiliser pour faire des mesures
 * Modifiez a vos risques et périls
 */
float takeValue(NewPing sonar){
  float sum = 0;                            
  int result;                               // Attention, si on ne fait pas la conversion directe, les valeurs sont tellement grandes qu'il faut utiliser un long 
  int taille = 0;
  while (taille < NB_MESURES){              // Tant que l'on a pas le bon nombre de mesures valides
    result = sonar.ping();
    //Serial.println(result);
    if (result != 0) {                      // Si la valeur est 0, elle n'est pas valide
      sum += result / US_ROUNDTRIP_CM;          // On convertis en distance avec la valeur donnée dans la librairie                              // On compte une mesure valide de plus
    }
    taille++;
    delay(DELAI_MESURES_CAPTEURS);          // Delai entre deux mesures pour éviter les interférences
  }
  return sum/NB_MESURES ;                   // On applique la moyenne
}

