#include <NMEAGPS.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>            //we are using UTFT display methods

#define gps_port Serial1
#define GPS_PORT_NAME "Serial1"
#define DEBUG_PORT Serial
#define TOUCH_ORIENTATION  PORTRAIT


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
  disp.clearScr();
	disp.print(String(sat_count),LEFT,0);
	disp.print(String(lon),CENTER,100);
	disp.print(String(lat),CENTER,140);
}
