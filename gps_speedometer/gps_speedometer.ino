#include <NeoHWSerial.h>  //enables hardware interrupts on gps serial port
#include <GPSport.h>      //switches on the above
#include <NMEAGPS.h>			//GPS library
#include <UTFTGLUE.h>     //we are using UTFT display methods
#include <SdFat.h>				//sd fat for SD card access


// Check configuration
#ifndef NMEAGPS_INTERRUPT_PROCESSING
  #error You must define NMEAGPS_INTERRUPT_PROCESSING in documents/arduino/libaries/neogps/NMEAGPS_cfg.h!
#endif


#define TZ_OFFSET  39600 //+11 hours. (+- hours*3600)
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

bool fix_current = false;

#include "definitions.h" //helper functions and etc.


//first thing is to "rename" the serial ports so they are easier to see apart
//these are for the leonardo which has two serial ports
#define GPS_SERIAL NeoSerial1
#define USB_SERIAL Serial

// ------------------------------------------------------------------
// ----------------------------------------  Store Status Information
// ------------------------------------------------------------------

info_t inf = {0,0,0,0,0,0,0,0,0,0}; 				//custom struct, check out definitions.h !

// ------------------------------------------------------------------
// ------------------------------------------------------------------

static void gps_isr(char c){                //gps interrupt service routine
	gps.handle(c);
}

// ------------------------------------------------------------------
// ---------------------------------------------------------  Setup()
// ------------------------------------------------------------------
void setup() {

	//initialise GPS and USB serials
	GPS_SERIAL.attachInterrupt(gps_isr);
	GPS_SERIAL.begin(9600);
	USB_SERIAL.begin(115200);

	//set up the screen and clear
	disp.InitLCD(PORTRAIT);
	disp.clrScr();
}
// ------------------------------------------------------------------
// ----------------------------------------------------------  Loop()
// ------------------------------------------------------------------
void loop() {


	//check for GPS fix information
	if(gps.available(GPS_SERIAL)){
		update_fields(gps.read()); //store fix into info_t structure
		inf.sat_count = gps.sat_count;
		fix_current = true;
	}

	if (inf.fixtime - millis() > 60000){
		fix_current = false;
	}

	draw_hid();
}
// ------------------------------------------------------------------
// ---------------------------------  draw_hid() and custom functions
// ------------------------------------------------------------------
void draw_hid()
{
	//topbar
	SET_BG_RED;
	SET_FG_WHT;
	disp.setTextSize(1);
	disp.print(F("Jaycar GPS Speedo              satt:    "),0,0);
	disp.printNumI(inf.sat_count,222,0);

	print_speed(100);
	print_time(60,200,4);

	print_date(60,250,4);			//date

	print_latlong(0,8);
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

	if(fix_current){
		SET_BG_BLK;
		SET_FG_BLK;
	}
	else{
		SET_BG_RED;
		SET_FG_YEL;
	}

	disp.setTextSize(8);
	disp.print("!",x,y);
}

void update_fields(const gps_fix &fix){ 

	if(fix.valid.location){ //valid location fix, update

		inf.lat = fix.latitudeL();
		inf.lon = fix.longitudeL();

		//speed and heading relies on location, so we can
		//update fixtime so that we know when the latest fix
		//has been relating to position.
		inf.fixtime = millis();
	}

	if(fix.valid.time){ //valid time fix, update

		//tz convert
		NeoGPS::clock_t seconds = fix.dateTime;
		seconds += TZ_OFFSET;
		NeoGPS::time_t dateTime = seconds;

		inf.hour 	= dateTime.hours;
		inf.minute = dateTime.minutes;
		inf.day	= dateTime.date;
		inf.month	= dateTime.month;
	}

	if(fix.valid.speed){
		inf.speed = fix.speed_kph();
	}

	if(fix.valid.heading){
		inf.heading = fix.heading_cd();
	}


}
