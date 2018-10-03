#include <NMEAGPS.h>			//GPS library
#include <SPI.h>				  //
#include <UTFTGLUE.h>     //we are using UTFT display methods
#include <SdFat.h>				//sd fat for SD card access
#include "macros.h"

//there's multiple things going on in this code

//first thing is to "rename" the serial ports so they are easier to see apart
//these are for the leonardo which has two serial ports
#define GPS_SERIAL Serial1
#define USB_SERIAL Serial

//These following lines are for SD card access
#define USE_SDIO 0
SdFatSoftSpi<12, 11, 13> sdcard; //define a Software SD interface.

//the leonardo does NOT have the regular SPI interface on pins 11,12,13.
//our shield expects there to be such, so we will use a "SoftSPI" interface
//to emulate. this is slower but it still works.
//arduino MEGA has the same problem (spi is on 51,52,53)
//arduino uno does not have this problem. (spi is on 11,12,13 as expected)

//button +text position
#define BUTTON_POSITION 20,250,220,300 
#define BUTTON_TEXT_POSITION 30,265

#define REDRAW_HID 2
#define REDRAW_BUTTON 1

//global variables
UTFTGLUE disp(0x9341, A2, A1, A3, A4, A0);	//display module shield
NMEAGPS gps;								//gps module
gps_fix current_fix;						//a "fix" is a gps reading of a fixed position.



//store some status information ####################################

//position / speed
long latitude = 0;			//long
long longitude = 0;			//lat
float speed = 0;		
long heading = 0;			//direction
short sat_count = 0;

unsigned long fixtime;

int hour = 0;
int minute = 0 ; 
int day = 0;
int month = 0;

#define REDRAW_HID 2
#define REDRAW_BUTTON 1

//status variables
#define get_bit(n,x)  ((x>>n)& 1) //look up bitmasks if you want to know what we're doing here
#define set_bit(n,x,b) (x &= ((~(1&b))<<n))
byte status_reg;
//following 8 bits maxium:
#define S_FIX_CURRENT 0
#define S_TRIP_STARTED 1
#define S_SDCARD 2
#define S_NULL 3
#define S_NULL 4
#define S_NULL 5
#define S_NULL 6
#define S_REDRAW 7

//define draw commands, we will fill them out later
void draw_hid();
void draw_button();

void setup() {

	//initialise GPS and USB serials
	GPS_SERIAL.begin(9600);
	USB_SERIAL.begin(115200);

	if(sdcard.begin(10)){ //pin 10 for SD card from shield.
    set_bit(status_reg,S_SDCARD,1);
	}
	//set up the screen and clear
	disp.InitLCD(PORTRAIT);
	disp.clrScr();
  
  DEBUG_FILE = sdcard.open("debug.txt",FILE_WRITE);

}

void loop() {

	//update the GPS Fix information
/*
	while(gps.available(GPS_SERIAL)){	//information on the GPS lines
		current_fix = gps.read(); 		//read the gps snapshot
*/
	

	while(GPS_SERIAL.available()){
		byte c = GPS_SERIAL.read();
		//print to file
		if(DEBUG_FILE){
			DEBUG_FILE.print(c);
		}	
		//send to gps
		if(gps.decode(c) != NMEAGPS::DECODE_COMPLETED)
			continue;

		//---
		//STORE INFORMATION
		sat_count = gps.sat_count;		//get the number of satellites in range


		if(current_fix.valid.location){ // check if the location is valid
			latitude = current_fix.latitudeL();
			longitude = current_fix.longitudeL();
			fixtime = millis();			//get the current time that we got this information
		}
/*
		if(current_fix.valid.time){
			hour = current_fix.dateTime.hours;
			minute = current_fix.dateTime.minutes;
			day = current_fix.dateTime.date;
			month = current_fix.dateTime.month;
			redraw = REDRAW_HID;
		}
*/
		if(current_fix.valid.speed)
			speed = current_fix.speed_kph();

		if(current_fix.valid.heading)
			heading = current_fix.heading_cd();
		
	}
  
  draw_hid();
  draw_button();
}

// -------------------------------------------------------------------
// draw_hid()
// Draws the status information 
// -------------------------------------------------------------------
void draw_hid(){
  /*
	String topbar = "DUINOTECH GPS " + String((status_sdcard?"Card":"NoCard")) + "   Satellites: " + String(sat_count);

	//we include spaces to clear out previous information
	//String position = "lon: " + String(longitude) + "\nlat: " + String(latitude);

	//String current_speed = String(speed);
	//String time = String(hour) + ":" + String(minute);
	//String date = String(day) + "/" + String(month);

	//red top bar
	SET_BG_RED;
	SET_FG_RED;
	disp.fillRect(0,0,disp.width(),9); //top bar
	disp.setTextSize(1);
	SET_FG_WHT;
	disp.print(topbar.c_str(), LEFT, 1);

	//yellow/gray location
	SET_BG_GRY;
	SET_FG_GRY;
	disp.fillRect(LEFT,10,disp.width(),25);
  SET_FG_YEL;
	disp.print(position.c_str(), LEFT, 10);


	//white speed
	SET_BG_BLK;
	SET_FG_WHT;
	disp.setTextSize(7);
	disp.print(current_speed.c_str(),LEFT,70);
	disp.setTextSize(3);
	disp.print(" kmph",100,140);

	SET_FG_YEL;
	disp.setTextSize(3);
	disp.print(time.c_str(),LEFT,200);
	SET_FG_CYA;
	disp.print(date.c_str(),150,200);
*/
}

void draw_button(){

	if(status_trip_started){
		SET_FG_RED;
		SET_BG_RED;
	}
	else{
		SET_FG_GRN;
		SET_BG_GRN;
	}

	disp.fillRect(BUTTON_POSITION);
	SET_FG_WHT;
	disp.setTextSize(3);
	disp.print(status_trip_started? "  End Trip":"Start Trip", //inline if statement: (question? "this if true" : "this if false")
			BUTTON_TEXT_POSITION); //position of text

	redraw = 0;	
}

