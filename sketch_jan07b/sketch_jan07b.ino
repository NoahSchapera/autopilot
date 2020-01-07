/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 *
 * Based on the example TinyGPS++ from arduiniana.org
 *
 */
 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

double CraftLat = 0;
double CraftLong = 0;

double TargetLat = 0;

double TargetLong = 0;

double bearing = 0;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void getBearing(double latNow, double longNow, double destinLat, double destinLong)
{
  double X = cos(destinLat)*sin(destinLong-longNow);
  double Y = cos(latNow)*sin(destinLat)-sin(latNow)*cos(destinLat)*cos(destinLong-longNow);
  bearing = (180*atan2(X,Y))/3.1415;
}


void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      // Latitude in degrees (double)
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);      
      // Longitude in degrees (double)
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6); 
            
      Serial.print("Course in degrees = "); 
      Serial.println(gps.course.deg()); 
      
      // Altitude in meters (double)
      Serial.print("Altitude in meters = "); 
      Serial.println(gps.altitude.meters()); 

    }
  }
}
