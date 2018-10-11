
#include <NMEAGPS.h>			//GPS library
#include <SPI.h>				  //
#include <UTFTGLUE.h>     //we are using UTFT display methods
#include <SdFat.h>				//sd fat for SD card access


// ------------------------------------------------------------------
// ------------------------------------------ Global Module Variables
// ------------------------------------------------------------------

//These following lines are for SD card access
#define USE_SDIO 0
SdFatSoftSpi<12, 11, 13> sdcard; //define a Software SD interface.

//display shield
UTFTGLUE disp(0x9341, A2, A1, A3, A4, A0);

//gps module
NMEAGPS gps;

#include "definitions.h"
// ------------------------------------------------------------------
// ----------------------------------------  Store Status Information
// ------------------------------------------------------------------

gps_fix latest_fix;			//a "fix" is a gps reading of a fixed position.
info_t inf = {0,0,0,0,0,0,0,0,0,0}; 				//custom struct, check out definitions.h !

// ------------------------------------------------------------------
// ------------------------------------------------------------------


void setup() {

	//initialise GPS and USB serials
	GPS_SERIAL.begin(9600);
	USB_SERIAL.begin(115200);

	if(sdcard.begin(10)){ //pin 10 for SD card from shield.

		set_status(S_SDCARD); //we have card access

	}
	//set up the screen and clear
	disp.InitLCD(PORTRAIT);
	disp.clrScr();
}

void loop() {


	//check for GPS fix information
	while(gps.available(GPS_SERIAL))
	{
		latest_fix = gps.read();
		update_fields(&latest_fix); //store fix into info_t structure
		inf.sat_count = gps.sat_count;
	}
status_r = 0xFF;

	if(get_status(S_SDCARD)){
		//write information to SD card
	}

	if(get_status(S_TRIP_STARTED)){
		//trip information
	}

	draw_hid();
}

void draw_hid()
{

	//topbar
	SET_BG_RED;
	SET_FG_WHT;
	disp.setTextSize(1);
	disp.print(F("Jaycar GPS Speedo              satt:    "),0,0);
	disp.printNumI(inf.sat_count,222,0);


	//if(get_status(D_SPEED))			//speed
		print_speed(100);

	if(get_status(D_TIME))			//time
		print_time(0,200,4);

	print_date(120,200,4);			//date

	if(get_status(D_LATLON))		//lat long
		print_latlong(0,8);

	if(get_status(D_BUTTON))		//button
		draw_button();
	
	print_warning(192,24);			//warning sign
}

void print_time(unsigned short x, unsigned short y,unsigned short scale){
	SET_FG_BLU;
  SET_BG_BLK;
	disp.setTextSize(scale);
	disp.printNumI(inf.hour,
		x + (inf.hour>9? 0: CHAR_WIDTH*scale), //conditionally move position so single digit is closer
		y);

	disp.print(":",x + CHAR_WIDTH*scale*2, y);

	if(inf.minute < 10){
		disp.print("0",x+CHAR_WIDTH*scale*3,y);
		disp.printNumI(inf.minute,x+CHAR_WIDTH*scale*4,y);
	} else {
		disp.printNumI(inf.minute,x+CHAR_WIDTH*scale*3,y);
	}

}
void print_date(unsigned short x, unsigned short y,unsigned short scale){
	SET_FG_GRN;
  SET_BG_BLK;
	disp.setTextSize(scale);
	disp.printNumI(inf.day,
		x + (inf.day>9? 0: CHAR_WIDTH*scale), //conditionally move position so single digit is closer
		y);

	disp.print("/",x + CHAR_WIDTH*scale*2, y);

	disp.printNumI(inf.month,x+CHAR_WIDTH*scale*3,y);

}
void print_speed(unsigned short y){
	SET_BG_BLK;
	SET_FG_WHT;
	disp.setTextSize(8);
	disp.printNumF(inf.speed,1,0,y); //one decimal point

	disp.setTextSize(2);
	disp.print(F("kmph"),SCREEN_WIDTH-CHAR_WIDTH*8,
						y+8*CHAR_HEIGHT); //8 here refers to setTextSize(8)
}

void print_latlong(unsigned short x, unsigned short y){
	
	SET_BG_GRY;
	SET_FG_YEL;
	disp.setTextSize(1);
	disp.print(F(" Latitude:"),x,y);
	disp.print(F("Longitude:"),x,y+CHAR_HEIGHT);

	disp.printNumF(inf.lat / 1000000, 7, x+10*CHAR_WIDTH,y);
	disp.printNumF(inf.lon / 1000000, 7, x+10*CHAR_WIDTH,y+CHAR_HEIGHT);
}

void print_warning(unsigned short x, unsigned short y){

	if(get_status(S_FIX_CURRENT))
		return;
	
	SET_BG_RED;
	SET_FG_YEL;
	disp.setTextSize(8);
	disp.print("!",x,y);
}

void update_fields(gps_fix *fix){ 
	/* ----
	 * To understand this code a little more, read up on C pointers and addresses;
	 */

	if(fix->valid.location){ //valid location fix, update
		inf.lat = fix->latitudeL();
		inf.lon = fix->longitudeL();

		//speed and heading relies on location, so we can
		//update fixtime so that we know when the latest fix
		//has been relating to position.
		
		inf.fixtime = millis();
	}
	
	if(fix->valid.time){ //valid time fix, update
		inf.hour 	= fix->dateTime.hours;
		inf.minute = fix->dateTime.minutes;
		inf.day	= fix->dateTime.date;
		inf.month	= fix->dateTime.month;
	}

	if(fix->valid.speed){
		inf.speed = fix->speed_kph();
	}

	if(fix->valid.heading){
		inf.heading = fix->heading_cd();
	}


}

void draw_button(){
	disp.setTextSize(3);
	SET_BG_GRN;
	SET_FG_GRN;
	disp.fillRect(BUTTON_POSITION);
	SET_FG_WHT;
	disp.print("Start Trip",BUTTON_TEXT_POSITION);
}
