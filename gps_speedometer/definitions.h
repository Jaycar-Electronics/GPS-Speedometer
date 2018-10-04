#ifndef _DEFINITIONS_H__
#define _DEFINITIONS_H__ 

#include <Arduino.h> 


// this is a macro file to define some quick and easy commands

//first thing is to "rename" the serial ports so they are easier to see apart
//these are for the leonardo which has two serial ports
#define GPS_SERIAL Serial1
#define USB_SERIAL Serial


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

#define get_status(n)  ((status_r>>n)& 1) //look up bitmasks if you want to know what we're doing here
#define set_status(n) (status_r |= (1<<n))
#define clr_status(n) (status_r &= ~(1<<n))
#define tgl_status(n) (status_r ^= (1<<n))

//following 8 bits maxium:
#define S_FIX_CURRENT 0
#define S_TRIP_STARTED 1
#define S_SDCARD 2
#define S_NULL 3
#define S_NULL 4
#define S_NULL 5
#define S_REDRAW_BACK 6
#define S_REDRAW_TEXT 7

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

//define draw commands, we will fill them out in functions.c
void draw_hid();
void draw_button();
void update_fields();


#endif
