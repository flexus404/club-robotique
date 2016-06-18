/* ENCODEURS
 * Auteur: Lorin MACE et Internet
 */
 
#include <digitalWriteFast.h> //Cette librairie permet de lire plus rapidement les données binaires

/* 
 *  PINS Codeur => Deux phases, A et B
 *  Attention: le fil rouge n'est pas forcément VCC ou +5, il peut être une phase
 *  Dans notre cas, le VCC était le fil blanc, GND noir, la phase A le rouge et la phase B le vert
 *  
 *  Les pins des encodeurs doivent être sur des pins qui supportent les interuptions, comme 2,3,20,21 voir le setup pour plus d'infos
 *  et la doc de attachInterrupt();
 */
#define Pin1A 2    //  1ere sortie du codeur1
#define Pin1B 3    //  2e sortie du codeur1

#define Pin2A 21    //  1ere sortie du codeur2
#define Pin2B 20    //  2e sortie du codeur2

/*
 * CONSTANTES
 */
float diametre = 7.635;    //Diametre de la roue
#define NBR_TICKS 600.0    //Nombres de ticks de l'encodeur par tour, dépend du modèle 

/*
 * Variables globales
 */
volatile double distance1 = 0;   //Distances Parcourues
volatile double distance2 = 0;
long long int ticks1 = 0;        //Nombre de ticks parcourus, les valeurs sont grandes donc un int ne passe pas
long long int ticks2 = 0;  

void setup ()  {
  
  Serial.begin (9600);   // initialisation du moniteur série

  /*
   * SETUP Encodeurs, les modes des pins
   */
  pinMode(Pin1A,INPUT);
  pinMode(Pin1B,INPUT); 
  pinMode(Pin2A,INPUT);
  pinMode(Pin2B,INPUT);  

  /*
   * On attache les interruptions sur les pins 2 et 21
   */
  attachInterrupt (0,routineEncodeur1,FALLING);   // interruption sur front descendant pin 2 => Voir fin programme
  attachInterrupt (3,routineEncodeur2,FALLING);   // interruption sur front descendant pin 21 => Voir fin programme
  //ATTENTION: Il faut lire la doc de cette fonction pour l'utiliser, ce n'est pas intuitif (on peut consulter la doc sans internet)

  Serial.println("Fin d'initialisation");
}

void loop ()  {
  //Calcul distance
  distance1=(diametre*PI)/(NBR_TICKS)*ticks1;
  distance2=(diametre*PI)/(NBR_TICKS)*ticks2;

  //Afficher la distance parcourue (facultatif)
  Serial.print("Distance 1 : ");
  Serial.print(distance1);
  Serial.print(" Distance 2 : ");
  Serial.println(distance2);
} 


/*
 * Interruptions des codeurs
 * Pour savoir ce qu'il se passe exactement, regardez l'explication sur wikipedia
 * Sinon, copiez-collez
 * 
 * En cas d'inversement, ne vous embêtez pas à inverser les pins, 
 * changes les -- par des ++ et inversement au sein d'une même routine
 */
// routine déclanchée quand le signal A passe de haut a bas encodeur 1
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
