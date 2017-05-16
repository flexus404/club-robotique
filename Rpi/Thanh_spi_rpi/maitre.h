#pragma once

/* ===== DEFINES =====*/

/* Le numero de Chip Select (CS) de l'esclave (Arduino) a activer 
   Valeurs possibles selons la doc : 
   BCM2835_SPI_CSX avec X dans {0,1,2}
   BCM2835_SPI_CS_NONE : manuel */
#define CS_SLAVE BCM2835_SPI_CS0

/* Bit ordering */
//#define BIT_ORDER BCM2835_SPI_BIT_ORDER_MSBFIRST
#define BIT_ORDER BCM2835_SPI_BIT_ORDER_LSBFIRST

/* Vitesse de l'horloge SPI : XXX une puissance de 2 ent re 2**8 et 2**16 */
//#define CLK_SPEED BCM2835_SPI_CLOCK_DIVIDER_XXX
#define CLK_SPEED BCM2835_SPI_CLOCK_DIVIDER_4096 //61kHz

/* Init CPOL et CPHA*/
#define PI_MODE BCM2835_SPI_MODE0  //CPOL = 0, CPHA = 0
//#define BCM2835_SPI_MODE1
//#define BCM2835_SPI_MODE2
//#define BCM2835_SPI_MODE3

/* ===== VARIABLES GLOBALES ===== */


