#include <NeoHWSerial.h>  //enables hardware interrupts on gps serial port
#include <GPSport.h>      //switches on the above
#include <NMEAGPS.h>			//GPS library
#include <SdFat.h>				//sd fat for SD card access


// Check configuration
#ifndef NMEAGPS_INTERRUPT_PROCESSING
#error You must define NMEAGPS_INTERRUPT_PROCESSING in documents/arduino/libaries/neogps/NMEAGPS_cfg.h!
#endif
#ifdef ARDUINO_AVR_UNO
#error Compiling for the wrong board! change board to Arduino Leonardo or yun
#endif


//first thing is to "rename" the serial ports so they are easier to see apart
//these are for the leonardo which has two serial ports
#define GPS_SERIAL NeoSerial1
#define USB_SERIAL Serial

//These following lines are for SD card access
#define USE_SDIO 0
SdFatSoftSpi<12, 11, 13> sdcard; //define a Software SD interface.

static File file_handle;
static gps_fix fix;
static NMEAGPS gps;
bool serial_test = false;
bool touch = true;

static void gps_isr(char c){
	gps.handle(c);
}

void start_kml(const char *trip_name){

	if(file_handle){
		file_handle.println(F("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
		file_handle.println(F("<kml xmlns=\"http://www.opengis.net/kml/2.2\">"));
		file_handle.println(F("\t<Document>"));
		file_handle.print(F("\t\t<name>"));
		file_handle.print(trip_name);
		file_handle.println(F("</name>"));
		file_handle.println(F("\t\t<description>You GPS trip</description>"));
		file_handle.println(F("\t\t<Style id=\"yellowLineGreenPoly\">"));
		file_handle.println(F("\t\t\t<LineStyle>"));
		file_handle.println(F("\t\t\t\t<color>7f00ffff</color>"));
		file_handle.println(F("\t\t\t\t<width>4</width>"));
		file_handle.println(F("\t\t\t</LineStyle>"));
		file_handle.println(F("\t\t\t<PolyStyle>"));
		file_handle.println(F("\t\t\t\t<color>7f00ff00</color>"));
		file_handle.println(F("\t\t\t</PolyStyle>"));
		file_handle.println(F("\t\t</Style>"));
		file_handle.println(F("\t\t<Placemark>"));
		file_handle.println(F("\t\t\t<name>Your GPS trip</name>"));
		file_handle.println(F("\t\t\t<description>Recorded by GPS speedo by Jaycar Electronics</description>"));
		file_handle.println(F("\t\t\t<styleUrl>#GPSRecord</styleUrl>"));
		file_handle.println(F("\t\t\t<LineString>"));
		file_handle.println(F("\t\t\t\t<extrude>1</extrude>"));
		file_handle.println(F("\t\t\t\t<tessellate>1</tessellate>"));
		file_handle.println(F("\t\t\t\t<altitudeMode>absolute</altitudeMode>"));
		file_handle.println(F("<coordinates>"));
	}
	else{
		Serial.println(F("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
		Serial.println(F("<kml xmlns=\"http://www.opengis.net/kml/2.2\">"));
		Serial.println(F("\t<Document>"));
		Serial.print(F("\t\t<name>"));
		Serial.print(trip_name);
		Serial.println(F("</name>"));
		Serial.println(F("\t\t<description>You GPS trip</description>"));
		Serial.println(F("\t\t<Style id=\"yellowLineGreenPoly\">"));
		Serial.println(F("\t\t\t<LineStyle>"));
		Serial.println(F("\t\t\t\t<color>7f00ffff</color>"));
		Serial.println(F("\t\t\t\t<width>4</width>"));
		Serial.println(F("\t\t\t</LineStyle>"));
		Serial.println(F("\t\t\t<PolyStyle>"));
		Serial.println(F("\t\t\t\t<color>7f00ff00</color>"));
		Serial.println(F("\t\t\t</PolyStyle>"));
		Serial.println(F("\t\t</Style>"));
		Serial.println(F("\t\t<Placemark>"));
		Serial.println(F("\t\t\t<name>Your GPS trip</name>"));
		Serial.println(F("\t\t\t<description>Recorded by GPS speedo by Jaycar Electronics</description>"));
		Serial.println(F("\t\t\t<styleUrl>#GPSRecord</styleUrl>"));
		Serial.println(F("\t\t\t<LineString>"));
		Serial.println(F("\t\t\t\t<extrude>1</extrude>"));
		Serial.println(F("\t\t\t\t<tessellate>1</tessellate>"));
		Serial.println(F("\t\t\t\t<altitudeMode>absolute</altitudeMode>"));
		Serial.println(F("<coordinates>"));
		serial_test = true;
	}
}

void write_kml(float lat, float lon){
	String line = String(fix.longitude(),7) + "," + String(fix.latitude(),7) + ",550";
	if(file_handle)
		file_handle.println(line.c_str());
	else
		Serial.println(line.c_str());
}

void end_kml(){
	if(file_handle){
		file_handle.println(F("</coordinates>"));
		file_handle.println(F("</LineString>"));
		file_handle.println(F("</Placemark>"));
		file_handle.println(F("</Document>"));
		file_handle.println(F("</kml>"));
	}else{
		Serial.println(F("</coordinates>"));
		Serial.println(F("</LineString>"));
		Serial.println(F("</Placemark>"));
		Serial.println(F("</Document>"));
		Serial.println(F("</kml>"));
	}
}
void setup() {

	//initialise GPS
	GPS_SERIAL.attachInterrupt(gps_isr);
	GPS_SERIAL.begin(9600);
	
	sdcard.begin(10);

}

void loop() {
	
	if(gps.available()){
		fix = gps.read();

		if(fix.valid.location){		//location alright, so we can log if needed

			write_kml(fix.latitude(),fix.longitude());
		}
	}

	if (touch){
		
		if(file_handle){	//we have a file, so save and close

			end_kml();
			file_handle.close();

		}else{
			if(!fix.valid.time){ //don't start if we don't have valid time
				Serial.println("don't have valid time");
				delay(100);
				return;
			}
			String filename = String(fix.dateTime.year) + String(fix.dateTime.month) + String(fix.dateTime.day) + "-" +
				String(fix.dateTime.hours) +":"+String(fix.dateTime.minutes)+".kml";

			file_handle =	sdcard.open(filename.c_str(),FILE_WRITE);
			if(!file_handle)
				Serial.println(filename);
			start_kml(filename.c_str());
			touch = false;
		}
	}
	
	
}
