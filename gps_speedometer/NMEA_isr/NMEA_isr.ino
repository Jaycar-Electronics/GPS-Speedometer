#include <NMEAGPS.h>

//======================================================================
//
//
//  Description:  This minimal program parses the GPS data during the 
//     RX character interrupt.  The ISR passes the character to
//     the GPS object for parsing.  The GPS object will add gps_fix 
//     structures to a buffer that can be later read() by loop().

#include <NeoHWSerial.h>
#include <Streamers.h>
#include <GpsPort.h>
// Check configuration

#ifndef NMEAGPS_INTERRUPT_PROCESSING
  #error You must define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

static NMEAGPS   gps;

//--------------------------

static void GPSisr( uint8_t c )
{
  gps.handle( c );

} // GPSisr

//--------------------------

void setup()
{
  NeoSerial.begin(9600);
  while (!NeoSerial)
    ;

  NeoSerial.print( F("NMEA_isr.INO: started\n") );
  NeoSerial.print( F("fix object size = ") );
  NeoSerial.println( sizeof(gps.fix()) );
  NeoSerial.print( F("NMEAGPS object size = ") );
  NeoSerial.println( sizeof(gps) );
  NeoSerial.println( F("Looking for GPS device on " GPS_PORT_NAME) );

  trace_header( NeoSerial );

  NeoSerial.flush();

  NeoSerial1.attachInterrupt(GPSisr);
  
  NeoSerial1.begin( 9600 );
}

//--------------------------

void loop()
{
  if (gps.available()) {
    // Print all the things!
    trace_all( NeoSerial, gps, gps.read() );
  }

  if (gps.overrun()) {
    gps.overrun( false );
    NeoSerial.println( F("DATA OVERRUN: took too long to print GPS data!") );
  }
}
