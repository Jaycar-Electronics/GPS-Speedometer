#include <NMEAGPS.h>
#include <SPI.h>
//#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>            //we are using UTFT display methods
//#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define gps_port Serial1
#define GPS_PORT_NAME "Serial1"
#define DEBUG_PORT Serial
#define TOUCH_ORIENTATION  PORTRAIT


#define SET_BG_RED  disp.setBackColor(255, 0, 0)
#define SET_BG_BLU  disp.setBackColor(0, 0, 255)
#define SET_BG_GRN  disp.setBackColor(0, 255, 0)
#define SET_BG_CYA  disp.setBackColor(0, 255, 255)
#define SET_BG_MAG  disp.setBackColor(255, 0, 255)
#define SET_BG_YEL  disp.setBackColor(255, 255, 0)
#define SET_BG_WHT  disp.setBackColor(255, 255, 255)
#define SET_BG_BLK  disp.setBackColor(0, 0, 0)


#define SET_FG_RED  disp.setColor(255, 0, 0)
#define SET_FG_BLU  disp.setColor(0, 0, 255)
#define SET_FG_GRN  disp.setColor(0, 255, 0)
#define SET_FG_CYA  disp.setColor(0, 255, 255)
#define SET_FG_MAG  disp.setColor(255, 0, 255)
#define SET_FG_YEL  disp.setColor(255, 255, 0)
#define SET_FG_WHT  disp.setColor(255, 255, 255)
#define SET_FG_BLK  disp.setColor(0, 0, 0)

UTFTGLUE disp(0x9341, A2, A1, A3, A4, A0);

NMEAGPS gps;
gps_fix fix;

//some status information ####################################
long lat = -1;
long lon = -1;
short sat_count = -1;

void setup() {
  gps_port.begin(9600);
  Serial.begin(115200);
  disp.InitLCD(TOUCH_ORIENTATION);
  disp.clrScr();

  SET_FG_RED;
  disp.fillRect(0,0,disp.width(),10);
}

//readmore https://github.com/SlashDevin/NeoGPS/blob/master/extras/doc/Troubleshooting.md#printing-too-much


void loop() {
	while(gps.available(gps_port)){
		fix = gps.read(); //fix is a "fixed" configuration from the GPS. it's one whole reading.
		if(fix.valid.location){ // check if the location is valid
			lat = fix.latitudeL();
			lon = fix.longitudeL();
		}
		sat_count = gps.sat_count;
	}

	displayScreen();
	//calculateMPG();
}

void displayScreen(){
  
  
  SET_FG_WHT;
  SET_BG_RED;
  String topbar = "GPS FUEL ECONOMY, sats: " + String(sat_count) + "     ";
  
  disp.print(topbar.c_str(), LEFT, 1);
  
  disp.setBackColor(64, 64, 64);
  disp.setColor(255,255,0);
  String position = "lon: " + String(lon) + "  lat: " + String(lat);
  disp.print(position.c_str(), CENTER, 227);

}
