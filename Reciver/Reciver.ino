#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10); // (CE, CSN)

struct gps_info{
  double latitude;
  double longitude;
  unsigned long date;
  unsigned long time;
}gps_data;
const byte address[6] = "5RF24";
void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_MAX); 
  radio.setChannel(125);
  radio.openReadingPipe(0, address); 
  radio.startListening();
  Serial.begin(9600);
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);
}

void loop() { 
  if (radio.available()) {
    byte gpsData;
    radio.read(&gps_data, sizeof(gps_data));
    Serial.write((uint8_t*)&gps_data, sizeof(gps_data));
  }
}
