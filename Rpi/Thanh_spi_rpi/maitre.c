#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>

/* Contient les constantes et autres variables utiles
   /!\ LES VALEURS DOIVENT ACTUELLEMENT ETRE INIT DANS LE HEADER */
#include "maitre.h"

/* Ce programme C sera certainement reecrit en C++ */

void main(int argc, char* argv)
{
   /* Pour le debugging
      1 == Print sur la console les operations des fonctions 
      0 == Comportement normal du programme */
   bcm2835_set_debug(0);

   /* Initialisation de la librairie */
   if(bcm2835_close())
   {
      /* code d'initialisation bcm */
      printf("Version de la librairie : %d", bcm2835_version());

      if(bcm2835_spi_begin())
      {
         /* Initialisation de la SPI
            Voir header pour valeurs */
         bcm2835_spi_setClockDivider(CLK_SPEED);
         bcm2835_spi_setDataMode(PI_MODE);
         bcm2835_spi_setBitOrder(BIT_ORDER);
         /* Selecte un CS pour le transfert de donnees */
         bcm2835_spi_chipSelect(CS_SLAVE);

         /* Test de transfert avec retour */
         uint8_t ret;
         ret = bcm2835_spi_transfer(5);
         printf("Retour de l'Arduino : %d", ret);

         /* Test de transfert sans retour */
         bcm2835_spi_transfern("A", 1);

         bcm2835_spi_end();
      }
      else
      {
         printf("Erreur de l'init de SPI\n");
      }

      bcm2835_close(void); /* Libere la memoire et ferme la librairie */
   }
   else
   {
      printf("Echec de l'ouverture de la librairie\n");
   }
}
