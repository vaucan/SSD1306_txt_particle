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

// init() is from:
// http://robotcantalk.blogspot.se/2015/03/interfacing-arduino-with-ssd1306-driven.html
// by Sonal Pinto 

#include "oled_ssd1306.h"

void OLED_init() {

	// Init the I2C interface (pins A4 and A5 on the Arduino Uno board) in Master Mode.
	Wire.begin();
	// keywords:
	// SEG = COL = segment = column byte data on a page
	// Page = 8 pixel tall row. Has 128 SEGs and 8 COMs
	// COM = row

	// Begin the I2C comm with SSD1306's address (SLA+Write)
	Wire.beginTransmission(OLED_I2C_ADDRESS);

	// Tell the SSD1306 that a command stream is incoming
	Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);

	// Follow instructions on pg.64 of the dataSheet for software configuration of the SSD1306
	// Turn the Display OFF
	Wire.write(OLED_CMD_DISPLAY_OFF);
	// Set mux ration tp select max number of rows - 64
	Wire.write(OLED_CMD_SET_MUX_RATIO);
	Wire.write(0x3F);
	// Set the display offset to 0
	Wire.write(OLED_CMD_SET_DISPLAY_OFFSET);
	Wire.write(0x00);
	// Display start line to 0
	Wire.write(OLED_CMD_SET_DISPLAY_START_LINE);
	
	// Mirror the x-axis. In case you set it up such that the pins are north.
	// Wire.write(0xA0); - in case pins are south - default
	Wire.write(OLED_CMD_SET_SEGMENT_REMAP);
	//Wire.write(0xA0);
		
	// Mirror the y-axis. In case you set it up such that the pins are north.
	// Wire.write(0xC0); - in case pins are south - default
	Wire.write(OLED_CMD_SET_COM_SCAN_MODE);
	//Wire.write(0xC0);
		
	// Default - alternate COM pin map
	Wire.write(OLED_CMD_SET_COM_PIN_MAP);
	Wire.write(0x12);
	// set contrast
	Wire.write(OLED_CMD_SET_CONTRAST);
	Wire.write(0x7F);
	// Set display to enable rendering from GDDRAM (Graphic Display Data RAM)
	Wire.write(OLED_CMD_DISPLAY_RAM);
	// Normal mode!
	Wire.write(OLED_CMD_DISPLAY_NORMAL);
	// Default oscillator clock
	Wire.write(OLED_CMD_SET_DISPLAY_CLK_DIV);
	Wire.write(0x80);
	// Enable the charge pump
	Wire.write(OLED_CMD_SET_CHARGE_PUMP);
	Wire.write(0x14);
	// Set precharge cycles to high cap type
	Wire.write(OLED_CMD_SET_PRECHARGE);
	Wire.write(0x22);
	// Set the V_COMH deselect volatage to max
	Wire.write(OLED_CMD_SET_VCOMH_DESELCT);
	Wire.write(0x30);
	// Horizonatal addressing mode - same as the KS108 GLCD
	Wire.write(OLED_CMD_SET_MEMORY_ADDR_MODE);
	Wire.write(0x00);
	// Turn the Display ON
	Wire.write(OLED_CMD_DISPLAY_ON);

	// End the I2C comm with the SSD1306
	Wire.endTransmission();
	
	OLED_clear();
}

void OLED_clear() {
    int k;
    
    OLED_set_pos(0, 0);
    for (k=0;k<128;k++) {
    	Wire.beginTransmission(OLED_I2C_ADDRESS);
    	Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
    	Wire.write(NULLVEC,FONTCOLS);
    	Wire.endTransmission();
    }
    OLED_set_pos(0, 0);
    
}

byte OLED_sprint(char *s, const char font[][FONTCOLS]) {
    // Print string to OLED display based on driver/controller SSD1306.
    // Returns number of bytes written.
    // Use this function even if printing a single character, do not use
    // printc()
    unsigned int len = strlen(s);
    char *p = s;
    byte res = 0; // number of bytes written
    
    while (*p != '\0') {
        // TODO check for '\n' and increase page by one
        Wire.beginTransmission(OLED_I2C_ADDRESS);
    	Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
    	res = res + printc(*p,font);
    	Wire.endTransmission();  
    	p++;
    }
    
    Serial.printlnf("chars in string: %u",len);
    Serial.printlnf("bytes written: %u",res);
    if (len*FONTCOLS != res)
        Serial.println("mismatch between number of chars in string and bytes written");
    
    
    return res/FONTCOLS;
}

void OLED_set_pos(byte y,byte x)  {
    // Set position of "cursor" on OLED display in terms of chars
    // (see driver/controller SSD1306 for definitions)
    // y is the "page" with range 0-7
    // x is multiples of FONTCOLS "segments", i.e., the width of a char depends on chosen font  
    Wire.beginTransmission(OLED_I2C_ADDRESS);
	Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
	Wire.write(OLED_CMD_SET_COLUMN_RANGE);
	Wire.write(FONTCOLS*x);
	Wire.write(0x7F);
	Wire.write(OLED_CMD_SET_PAGE_RANGE);
	Wire.write(y);
	Wire.write(0x07);
	Wire.endTransmission();
}
