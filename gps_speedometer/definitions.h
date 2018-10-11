#ifndef _DEFINITIONS_H__
#define _DEFINITIONS_H__ 

#include <Arduino.h> 


// this is a macro file to define some quick and easy commands

//first thing is to "rename" the serial ports so they are easier to see apart
//these are for the leonardo which has two serial ports

#define GPS_SERIAL Serial1
#define USB_SERIAL Serial


#define CHAR_WIDTH 6     //default GFX_lib values, do not change
#define CHAR_HEIGHT 8    
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320


// STRINGS
#define STR_TITLE   F("Duinotech GPS")
#define STR_CARD    F("Card")
#define STR_NOCARD  F("NoCard")
#define STR_LATITUDE  F("lat:")
#define STR_LONGITUDE F("lon:")
#define STR_KMPH      F("kmph")
#define STR_START_TRIP  F("Start Trip")
#define STR_STOP_TRIP   F("Stop Trip")


// COLOURS
#define SET_BG_RED  disp.setBackColor(255, 0, 0)
#define SET_BG_BLU  disp.setBackColor(0, 0, 255)
#define SET_BG_GRN  disp.setBackColor(0, 255, 0)
#define SET_BG_CYA  disp.setBackColor(0, 255, 255)
#define SET_BG_MAG  disp.setBackColor(255, 0, 255)
#define SET_BG_YEL  disp.setBackColor(255, 255, 0)
#define SET_BG_WHT  disp.setBackColor(255, 255, 255)
#define SET_BG_BLK  disp.setBackColor(0, 0, 0)
#define SET_BG_GRY  disp.setBackColor(100,100,100)
#define SET_FG_GRY  disp.setColor(100,100,100)
#define SET_FG_RED  disp.setColor(255, 0, 0)
#define SET_FG_BLU  disp.setColor(0, 0, 255)
#define SET_FG_GRN  disp.setColor(0, 255, 0)
#define SET_FG_CYA  disp.setColor(0, 255, 255)
#define SET_FG_MAG  disp.setColor(255, 0, 255)
#define SET_FG_YEL  disp.setColor(255, 255, 0)
#define SET_FG_WHT  disp.setColor(255, 255, 255)
#define SET_FG_BLK  disp.setColor(0, 0, 0)

//button +text position
#define BUTTON_POSITION 20,250,220,300 
#define BUTTON_TEXT_POSITION 30,265

//status variables
byte status_r = 0;

#define get_status(n)  (status_r& n) //look up bitmasks if you want to know what we're doing here
#define set_status(n) (status_r |= n)
#define clr_status(n) (status_r &= ~n)
#define tgl_status(n) (status_r ^= n)

//following 8 bits maxium:
#define S_FIX_CURRENT   (1<<1)
#define S_TRIP_STARTED  (1<<2)
#define S_SDCARD		(1<<3) 
#define S_NOTUSED			(1<<4)
//redraw routines
#define D_TIME  		(1<<5)
#define D_LATLON 		(1<<6)
#define D_BUTTON 		(1<<7)
#define D_SPEED 		(1<<8)

#define STR_BUF_SIZE 24


typedef struct {
	long lat;
	long lon;
	float speed;
	long heading;
	short sat_count;
	//age of fix
	unsigned long fixtime;
	//datetime
	unsigned short hour;
	unsigned short minute;
	unsigned short day;
	unsigned short month;
} info_t;

//define draw commands, we will fill them out in the main 
void draw_hid();
void draw_button();
void update_fields();


#endif
