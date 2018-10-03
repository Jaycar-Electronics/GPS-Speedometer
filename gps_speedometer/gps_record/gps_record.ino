#include <SPI.h>				  //
#include <SdFat.h>				//sd fat for SD card access

#define GPS_SERIAL Serial1
#define USB_SERIAL Serial

//These following lines are for SD card access
#define USE_SDIO 0
SdFatSoftSpi<12, 11, 13> sdcard; //define a Software SD interface.

File DEBUG_FILE;
void setup() {

	//initialise GPS and USB serials
	GPS_SERIAL.begin(9600);
	USB_SERIAL.begin(115200);

	sdcard.begin(10);
	

}
char buffer[24];
long c_time;

void loop() {
  c_time = millis();
  
	snprintf(buffer,24,"%d.txt",c_time);
	USB_SERIAL.println(buffer);
 
  DEBUG_FILE = sdcard.open(buffer,FILE_WRITE);

  if(!DEBUG_FILE)
    Serial.println("unable to open file");

  for(;;){
  	while(GPS_SERIAL.available()){
  		byte c = GPS_SERIAL.read();
     Serial.write(c);
  		if(DEBUG_FILE){
  			DEBUG_FILE.write(c);
  		}    
  	}
   if ((millis() - c_time) > 300000){
    DEBUG_FILE.close();
    Serial.println("---------------------------------closed file--------------------------------");
    break;
   }
	}
  
}
