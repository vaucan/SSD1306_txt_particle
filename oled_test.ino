/* The MIT License (MIT)

Copyright (c) 2016 vaucan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// This #include statement was automatically added by the Particle IDE.
#include "oled_ssd1306.h"

// This #include statement was automatically added by the Particle IDE.
#include "font.h"

#define SLEEP_MS 1*1e3

char buf[50];
byte res;

void setup() {  
  OLED_init();
  OLED_clear();
  
  Serial.begin(9600);
  Serial.println("init finished");
}

void loop()
{
    //OLED_clear();
    OLED_set_pos(0,0);
	sprintf(buf,"Hello World");
	res = OLED_sprint(buf, font_basic); 
	Serial.printlnf("chars written: %u",res);
    
    Serial.println("waiting...");
    delay(SLEEP_MS);               // in ms
}
