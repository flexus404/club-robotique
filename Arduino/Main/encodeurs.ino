//#include <digitalWriteFast.h> 

/*#define Pin1A 2    //  1ere sortie du codeur1
#define Pin1B 3    //  2e sortie du codeur1

#define Pin2A 21    //  1ere sortie du codeur2
#define Pin2B 20    //  2e sortie du codeur2

float diametre = 7.635;    //Diametre de la roue
#define NBR_TICKS 600.0    //Nombres de ticks de l'encodeur par tour, dépend du modèle 

volatile double distance1 = 0;   //Distances Parcourues
volatile double distance2 = 0;
long long int ticks1 = 0;        //Nombre de ticks parcourus, les valeurs sont grandes donc un int ne passe pas
long long int ticks2 = 0; 
/*
 * Interruptions des codeurs
 * Pour savoir ce qu'il se passe exactement, regardez l'explication sur wikipedia
 * Sinon, copiez-collez
 * 
 * En cas d'inversement, ne vous embêtez pas à inverser les pins, 
 * changes les -- par des ++ et inversement au sein d'une même routine

//routine déclanchée quand le signal A passe de haut a bas encodeur 1
void routineEncodeur1()  {     
  if (digitalReadFast2(Pin1A)){       //Si le pin1A est activé
    if (digitalReadFast2(Pin1B)) {    //Ainsi que le pin 1B => voir wikipédia => Cela permet de déterminer le sens de rotation
      ticks1++;                       
    } else {
      ticks1--;
    }
  } else { 
    if (digitalReadFast2(Pin1B)){
      ticks1--;
    } else {
      ticks1++;
    }
  }
}

// routine déclanchée quand le signal A passe de haut a bas encodeur 2
void routineEncodeur2()  {     
  if (digitalReadFast2(Pin2A)){
    if (digitalReadFast2(Pin2B)) {
      ticks2--;
    } else {
      ticks2++;
    }
  } else { 
    if (digitalReadFast2(Pin2B)){
      ticks2++;
    } else {
      ticks2--;
    }
  }
}

    distance1=(diametre*PI)/(NBR_TICKS)*ticks1;
    distance2=(diametre*PI)/(NBR_TICKS)*ticks2;

    //Afficher la distance parcourue (facultatif)
    Serial.print("Distance 1 : ");
    Serial.print(distance1);
    Serial.print(" Distance 2 : ");
    Serial.println(distance2);
  */

