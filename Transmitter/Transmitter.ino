#include <SPI.h>
#include <RF24.h>
#include <SoftwareSerial.h>
#include <NMEAGPS.h>
#include <GPSport.h>

RF24 radio(5, 6); // (CE, CSN)
const byte address[6] = "5RF24";
static NMEAGPS  gps;
struct gps_info{
  double latitude;
  double longitude;
  unsigned long date;
  unsigned long time;
}gps_data;
void setup() {
  //Setup radio
  radio.begin();
  radio.setPALevel(RF24_PA_MAX); 
  radio.setChannel(125);
  radio.openWritingPipe(address); 
  radio.stopListening();
  Serial.begin(9600);
  radio.setAutoAck(true);
  radio.setRetries(5,15);
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);
  //Setup NeoGPS
  gpsPort.begin(9600);

}

static void doSomeWork( const gps_fix & fix );
static void doSomeWork( const gps_fix & fix )
{

  if (fix.valid.location) {
    if ( fix.dateTime.seconds < 40 )
      gps_data.latitude=fix.latitude();
      gps_data.longitude=fix.longitude();
      radio.write(&gps_data, sizeof(gps_data));
  }


} // doSomeWork

static void GPSloop();
static void GPSloop()
{
  while (gps.available( gpsPort ))
    doSomeWork( gps.read() );

} // GPSloop


void loop() {
  GPSloop();
}
