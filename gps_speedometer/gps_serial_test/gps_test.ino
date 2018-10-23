#include <NMEAGPS.h>			//GPS library
#include <SPI.h>				

#define GPS_SERIAL Serial1
#define USB_SERIAL Serial


void setup() {

	//initials
	GPS_SERIAL.begin(9600);
	USB_SERIAL.begin(115200);

}

//readmore https://github.com/SlashDevin/NeoGPS/blob/master/extras/doc/Troubleshooting.md#printing-too-much


void loop() {

	if(GPS_SERIAL.available())
		USB_SERIAL.write(GPS_SERIAL.read());

	delay(1);
}

