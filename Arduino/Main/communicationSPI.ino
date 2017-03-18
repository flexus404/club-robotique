#include <SPI.h>

volatile byte pos;
char buf[64];

volatile boolean process_it;

int envoyer(char cmd[])
{
  
}

ISR (SPI_STC_vect)
{
  //On recupere le bit dans le registre de sonnée SPI
  byte c = SPDR; 
  
  if (pos < sizeof(buf))
    {
    buf [pos++] = c;
    
    if (c == '\n')
      process_it = true;
      
    }  
} 

