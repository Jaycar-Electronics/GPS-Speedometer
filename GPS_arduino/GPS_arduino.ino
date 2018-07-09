#include <NMEAGPS.h>
#include <SPI.h>
//#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>            //we are using UTFT display methods
//#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define FUEL_ECON_UNITS "L/100KM"


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


UTFTGLUE disp(0x9341, A2, A1, A3, A4, A0);
NMEAGPS gps;
gps_fix fix;

//some status information ####################################
long lat = -1;
long lon = -1;
short sat_count = -1;
float current_ECON = 0;

bool ECON_dirty = true;
bool BTN_dirty = true;
bool HID_dirty = true;
bool KP_dirty = true;
bool KP_mode = false;

void draw_kp();
void draw_econ();
void draw_btn();
void draw_hid();


void setup() {
	gps_port.begin(9600);
	Serial.begin(115200);
	disp.InitLCD(TOUCH_ORIENTATION);
	disp.clrScr();

	SET_FG_RED;
	disp.fillRect(0,0,disp.width(),9);

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

  processGPS();
	displayScreen();
}

void processGPS(){
  //here we write to a file,
  //and calculate MPG;

  int distance = random(5);
  int refuel = random(4);

  current_ECON = distance / refuel;

  //ECON_dirty = true;
}

void draw_hid(){

  String topbar = "GPS FUEL ECONOMY, satellites: " + String(sat_count) + "     ";
  String position = "lon: " + String(lon) + "                \nlat: " + String(lat) + "                ";
  disp.setTextSize(1);
  //red top bar
  SET_BG_RED;
  SET_FG_WHT;
  disp.print(topbar.c_str(), LEFT, 1);

  //yellow/gray location
  SET_BG_GRY;
  SET_FG_YEL;
  disp.print(position.c_str(), LEFT, 10);
  HID_dirty = false;
}

void draw_econ(){

  String econ = String(current_ECON,1);
  SET_FG_BLK;
  disp.fillRect(0,50,disp.width(),130);
  SET_BG_BLK;
  SET_FG_WHT;
  disp.setTextSize(9);
  disp.print(econ.c_str(),0,50);
  disp.setTextSize(3);
  disp.print(FUEL_ECON_UNITS,100,130);
  ECON_dirty = false;
}

void draw_btn(){
  //green refuel button
  SET_FG_GRN;
  disp.setTextSize(3);
  disp.fillRect(20,180,disp.width()-20,300);
  SET_FG_WHT;
  SET_BG_GRN;
  disp.print("refuel",66,220);
  BTN_dirty = false;
}
void displayScreen(){
  
  if(KP_mode){
    if(KP_dirty) draw_kp();
  } else {
    if(HID_dirty) draw_hid();
    if(BTN_dirty) draw_btn();
    if(ECON_dirty) draw_econ();
  }
  
}
