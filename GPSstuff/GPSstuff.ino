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

double CurrentBearing = 0;
double TargetBearing = 0;

int millisCounter = 0;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  if(millis()%10000 == 1)
  {
    while (ss.available() > 0)
    {
    gps.encode(ss.read());
    if (gps.location.isUpdated())
      {
      CraftLat = gps.location.lat();
      CraftLong = gps.location.lng();
      CurrentBearing = gps.course.deg()
      }
    }
    TargetBearing = getBearing(CraftLat,CraftLong,DestinLat,DestinLong)
    }
  }
}

double  getBearing(double latNow, double longNow, double destinLat, double destinLong)
{
  double X = cos(radians(destinLat))*sin(radians(destinLong-longNow));
  //cout << X << endl;
  double Y = cos(radians(latNow))*sin(radians(destinLat))-sin(radians(latNow))*cos(radians(destinLat))*cos(radians(destinLong-longNow));
  //cout << Y << endl;
  double bearing = (180*atan2(X,Y))/3.1415;
  return bearing;
}
