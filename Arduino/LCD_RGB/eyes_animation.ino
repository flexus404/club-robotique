/*
  Blink.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  Grove - Serial LCD RGB Backlight demo.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;


void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() 
{
  int i = 0;
  for (i = 0; i < 85; i++)
  {
    lcd.clear();    
    lcd.setRGB(50+i*200/85, 50, 250-i*200/85);
    if(i < 20)
    {
      lcd.print("     _    _     ");
      lcd.setCursor(0, 1);
      lcd.print("     o    o     ");  
      delay(1000);
    }
    else if(i < 40)
    {
      lcd.print("     ^    ^     ");
      lcd.setCursor(0, 1);
      lcd.print("     o    o     ");  
      delay(1000);      
    }
    else if(i < 60)
    {
      lcd.print("     O    O     ");
      lcd.setCursor(0, 1);
      lcd.print("       []       ");  
      delay(1000);            
    }
    else
    {
      lcd.print("u    O    O    u");
      lcd.setCursor(0, 1);
      lcd.print(" |     []     | ");
      delay(500);
      lcd.clear();
      lcd.print(" u   O    O   u ");
      lcd.setCursor(0, 1);
      lcd.print(" |     []     | ");
      delay(500);
    }
  }
  for (i = 85; i < 90; i++)
  {
    lcd.clear();
    lcd.print("    ___  ___    ");
    lcd.setCursor(0, 1);
    lcd.print("   ( o    o )   ");
    int j = 0;
    for( j = 0; j < 4; j ++)
    {
      lcd.setRGB(255, 50, 50);
      delay(125);
      lcd.setRGB(0, 0, 0);
      delay(125);
    }
  }
  for (i = 90; i < 95; i++){
    lcd.clear();
    lcd.setRGB(255, 255, 255);
    delay(1000);
  }
  for (i = 95; i < 125; i++)
  {
    lcd.clear();
    lcd.setRGB(50, 50, 50+5*(i-90));
    lcd.print("      o  o     ");
    lcd.setCursor(0, 1);
    lcd.print("     `____/    ");
    delay(1000);
  }
  for (i = 125; i < 10000; i++)
  {
    lcd.setRGB(75, 75, 255);
    delay(1000);
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
