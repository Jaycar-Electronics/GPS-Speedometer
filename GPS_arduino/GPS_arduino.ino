#include <NMEAGPS.h>
#include <SPI.h>
//#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>            //we are using UTFT display methods
//#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define FUEL_ECON_UNITS "L/100KM"

// the button position form of x y x2 y2
#define BUTTON_POSITION 20,180,220,300 

#define gps_port Serial1
#define GPS_PORT_NAME "Serial1"
#define DEBUG_PORT Serial
#define TOUCH_ORIENTATION  PORTRAIT
#define XC4630_TOUCHX0 920
#define XC4630_TOUCHY0 950
#define XC4630_TOUCHX1 170
#define XC4630_TOUCHY1 200

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

  //update the GPS Fix information
  
	while(gps.available(gps_port)){
		fix = gps.read(); //fix is a "fixed" configuration from the GPS. it's one whole reading.
		if(fix.valid.location){ // check if the location is valid
			lat = fix.latitudeL();
			lon = fix.longitudeL();
		}
		sat_count = gps.sat_count;
	}

  //do what we need to do with GPS fix information
  processGPS(fix);

  //state machine: do what we need to in each mode.
  
  if(KP_mode){ //keypad mode, entering in refuel
    //process_keypad();
  }
  else
  { //normal mode, displaying MPG
    
     if(touch(BUTTON_POSITION)){
       //change to KP mode
       KP_mode = true;
     }

     
  }
 
  //handles the screen display depending on which mode
	displayScreen(KP_mode);
}

void processGPS(gps_fix fix){
  //here we write to a file,

  if(fix.valid.location )

  current_ECON = 0;
  ECON_dirty = true;
}

void draw_kp(){};

void draw_hid(){

  String topbar = "DUINOTECH FUELECON, satellites: " + String(sat_count) + "     ";
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
  disp.fillRect(BUTTON_POSITION);
  SET_FG_WHT;
  SET_BG_GRN;
  disp.print("refuel",66,220);
  BTN_dirty = false;
}
void displayScreen(bool KP_mode){

  //hid on both screens
  if(HID_dirty) draw_hid();
  
  if(KP_mode){
    
    if(KP_dirty) draw_kp();
    
  } else {
    
    if(ECON_dirty) draw_econ();
    if(BTN_dirty) draw_btn();

  }
}

int XC4630_touchrawx(){           //raw analog value
  int x;
  pinMode(8,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(8,LOW);            //doesn't matter if this changes
  digitalWrite(A2,HIGH);          //this is normally high between screen commands
  pinMode(A3,INPUT_PULLUP);       //this is normally high between screen commands
  pinMode(9,INPUT_PULLUP);        //doesn't matter if this changes
  analogRead(A3);                 //discard first result after pinMode change
  delayMicroseconds(30);
  x=analogRead(A3);
  pinMode(A3,OUTPUT);
  digitalWrite(A3,HIGH);          //restore output state from above
  pinMode(9,OUTPUT);
  return(x);  
}

int XC4630_touchrawy(){           //raw analog value
  int y;
  pinMode(9,OUTPUT);
  pinMode(A3,OUTPUT);
  digitalWrite(9,LOW);            //doesn't matter if this changes
  digitalWrite(A3,HIGH);          //this is normally high between screen commands
  pinMode(A2,INPUT_PULLUP);       //this is normally high between screen commands
  pinMode(8,INPUT_PULLUP);        //doesn't matter if this changes
  analogRead(A2);                 //discard first result after pinMode change
  delayMicroseconds(30);
  y=analogRead(A2);
  pinMode(A2,OUTPUT);
  digitalWrite(A2,HIGH);          //restore output state from above
  pinMode(8,OUTPUT);
  return(y);  
}

int XC4630_touchx(){
  int xc = map(XC4630_touchrawx(),XC4630_TOUCHX0,XC4630_TOUCHX1,0,240-1);
  if(xc>240-1){xc=-1;}         //off screen
  return xc;
}

int XC4630_touchy(){
  int yc = map(XC4630_touchrawy(),XC4630_TOUCHY0,XC4630_TOUCHY1,0,320-1);
  if(yc>320-1){yc=-1;}         //off screen
  return yc;
}

int touch(int x1,int y1,int x2,int y2){    //touch occurring in box?
  int x,y;
  x=XC4630_touchx();
  if(x<x1){return 0;}
  if(x>x2){return 0;}
  y=XC4630_touchy();
  if(y<y1){return 0;}
  if(y>y2){return 0;}
  return 1;
}

