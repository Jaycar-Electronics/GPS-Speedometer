#include <NMEAGPS.h>			//GPS library
#include <SPI.h>				
#include <UTFTGLUE.h>           //we are using UTFT display methods

#include "macros.h"

#define GPS_SERIAL Serial1
#define USB_SERIAL Serial

#define BUTTON_POSITION 20,250,220,300 
#define BUTTON_TEXT_POSITION 30,265

UTFTGLUE disp(0x9341, A2, A1, A3, A4, A0);	//display module
NMEAGPS gps;								//gps module
gps_fix current_fix;						//a "fix" is a gps reading of a fixed position.
gps_fix trip_start_fix;						//one to use later.

//some status information ####################################
long lat = -1;
long lon = -1;
short sat_count = -1;
bool trip_started = false;

//define draw commands, we will fill them out later
void draw_hid();
void draw_button();
bool dirty_b = true;


void setup() {

	//initials
	GPS_SERIAL.begin(9600);
	USB_SERIAL.begin(115200);
	disp.InitLCD(PORTRAIT);
	disp.clrScr();


}

//readmore https://github.com/SlashDevin/NeoGPS/blob/master/extras/doc/Troubleshooting.md#printing-too-much


void loop() {

	//update the GPS Fix information
	while(gps.available(GPS_SERIAL)){	//information on the GPS lines
		current_fix = gps.read(); 		//read the gps snapshot

		sat_count = gps.sat_count;		//get the number of satellites in range

		if(current_fix.valid.location){ // check if the location is valid
			lat = current_fix.latitudeL();
			lon = current_fix.longitudeL();
		}

		if(current_fix.valid.time){
		}

		if(current_fix.valid.altitude){
		}


	}

	draw_hid();

	if(dirty_b)
		draw_button();
}

// -------------------------------------------------------------------
// draw_hid()
// Draws the status information 
// -------------------------------------------------------------------
void draw_hid(){
	String topbar = "DUINOTECH GPS SPEEDO, satellites: " + String(sat_count) + "     ";
	String position = "lon: " + String(lon) + "                \nlat: " + String(lat) + "                ";

	//red top bar
	SET_BG_RED;
	SET_FG_RED;
	disp.fillRect(0,0,disp.width(),9); //top bar
	disp.setTextSize(1);
	SET_FG_WHT;
	disp.print(topbar.c_str(), LEFT, 1);

	//yellow/gray location
	SET_BG_GRY;
	SET_FG_YEL;
	disp.print(position.c_str(), LEFT, 10);
}

void draw_button(){

	if(trip_started){
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
	disp.print(trip_started? "  End Trip":"Start Trip", //inline if statement: (question? "this if true" : "this if false")
			BUTTON_TEXT_POSITION); //position of text					// ? = 'if' and ':' = 'else' 
	dirty_b = false;
} 
