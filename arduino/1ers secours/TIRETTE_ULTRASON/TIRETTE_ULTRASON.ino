/*
 * Auteur: Lorin MACE
 *
 * Votre tirette a lachée? Vous n'avez pas le temps de la réparer?
 * Utilisez l'un de vos capteurs ultrason comme démarage!
 * raspberry
 *    => Approuvée par la coupe de france 2016 =p
 *
 *
 * MATERIEL NECESSAIRE:
 * - Carton
 * - Gaffeur (scotch, gretep)
 * - Ficelle de taille homologable (50cm min en 2016)
 *
 *
 * FABRICATION:
 * 1) Coupez un bout de carton de la taille de votre capteur
 * 2) Percez le bout de carton et mettez une ficelle au travers
 * 3) Faites un noeud
 * 4) Entourez un bout de carton dans du gaffeur (gretep ou scotch)
 * 5) Laissez dépasser un bout de scotch légèrement collant de chaque coté pour pouvoir l'accrocher sur les bords du capteur
 *
 *
 * Placez le carton sur un capteur avant de préférence, car il faut que celui-ci puisse détecter une distance au départ
 *
 *
 * Installation: NewPing.h dans la librairie arduino + inclure ligne suivante
 *
 */
#include <NewPing.h>

// Rentrez ici les numeros de pin
#define trigPin 41
#define echoPin 40

//Création du capteur
NewPing sonarAvantGauche(trigPin, echoPin, MAX_DISTANCE);

/*
 * Fonction appelé pour savoir si le carton est enlevé du robot
 */
int initTrigger(NewPing sonar){
  int i;
  int value = 0;
  int uS;
  for(i=0;i<40;i++){                        // Lorsque l'on obtiens un certain nombre de valeur (ici 40) supérieure au seuil 
    uS = sonar.ping();                      // Mesure du capteur à ultrason (en ms)

    //Serial.print("resultat sonar");       // Servez vous de ces lignes pour régler le seuil de départ, prenez une petite marge
    //Serial.println(uS);

    if (uS < 20) {                          // On rentre le seuil ici  (ici 20)
      return 0;                             // Si on détecte une valeur inférieure au seuil, on retourne faux
    }
    //Si vous trouvez trop de valeurs incohérentes, vous pouvez mettre un délai ( delay(1); ) pour éviter les interférences (1 en ms)
  }
  return 1;                                 // Sinon on retourne vrai
}

/*
 * Booléen qui permet de savoir si on a démaré ou non
 */
bool started = false; //Au début, on a pas encore démaré (enfin je crois =p)

void setup() {
  Serial.begin(9600); //Afficher des valeurs sur la sortie standard
}

void loop() {
  delay(50);                            // Eviter la surchauffe de l'arduino (vous pouvez enlever pour avoir un programme plus réactif)

  if (!started){                        // Si on a pas encore démarée
    if (initTrigger(sonarAvantGauche)){ // Si le carton n'est plus sur le capteur
      started = true;                   // On démarre
    }
  } else {                              // Sinon, on a démaré

      /*
       * Programme de la coupe de la win
       */
  }
}


/*
 * Fun fact: La 1ere version du lanceur était uniquement avec du gaffeur et une ficelle,
 * pendant un match de coupe la ficelle s'est décrochée du gaffeur, et le robot n'a pas démarré xD
 *
 * CONSEIL: METTEZ DU CARTON !
 */
