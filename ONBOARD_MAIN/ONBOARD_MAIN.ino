#include <MPU6050_tockn.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>


//GPS VARS
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

//current lat long bearing
double CraftLat = 0;
double CraftLong = 0;
double CurrentBearing = 0;

//target lat long bearing
double TargetLat = 0;
double TargetLong = 0;
double TargetBearing = 0;



//RADIO VARS

RF24 receive (7,8);                         //create object called receive
byte address [5] = "00001";                 //creat an array with 5 elements, where each element is 1 byte;

//SERVO VARS

int TargetAngle = 0;
Servo Pitch;
Servo Yaw;
Servo Roll1;
Servo Roll2;
MPU6050 mpu6050(Wire);
int servomap = 0;
int HorizAngle = 0;
int PitchAngle = 0;
int YawAngle = 0;

void ServoSetup()
{
  Pitch.attach(8);
  Yaw.attach(9);
  Roll1.attach(10);
  Roll2.attach(11);
 
  Pitch.write(90);
  Yaw.write(90);
  Roll1.write(90);
  Roll2.write(90);

  
  
}

void MPUSetup()
{
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
}

void onlineBlink()
{
    for(int i = 0; i<3; i++)
  {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  }

}
void RecieveSetup()
{
  Serial.println("Starting Receiver");
  receive.begin();
  receive.openReadingPipe(0,address);      //open reading pipe 0 at address 00001
  receive.setPALevel(RF24_PA_MIN);         //Set RF output to minimum
  receive.setDataRate(RF24_250KBPS);       //set datarate to 250kbps
  receive.setChannel(100);                 //set frequency to channel 100
  receive.startListening();
}
void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  
  ServoSetup();
  MPUSetup();
  onlineBlink();
  RecieveSetup();
  ss.begin(GPSBaud);

  
}


void loop() 
{
  
  if (receive.available())                //check when received data available
  {
    char data[32];
    receive.read(&data, sizeof(data));
    //Serial.println(data);                   //print data to serial monitor
  sscanf(data, "%d", &TargetAngle);
  }

  if(millis()%10000 == 1)
  {
    encodePosition();
    TargetBearing = getBearing(CraftLat,CraftLong,DestinLat,DestinLong);
  }
  
  grabAngles();
  printAngles();


  //*NOTE FOR NOAH TOMMOROW - TODO: SEND A LATLONG INSTEAD OF ANGLE, PARSE LATLONG, STORE, SEND TO BEARING CALC, COMPARE ANGLE TO DESIRED BEARING
    *
  for(int c = 0; c < 3; c++)
  {
    switch(c)
    {
      case 0:
        servomap = map(PitchAngle, -90, 90, 0, 180);
        Pitch.write(servomap);
        break;
      case 1:
        if(YawAngle-TargetAngle>5)
        {
          Yaw.write(map(45,-90,90,0,180));
        }
        if(YawAngle-TargetAngle<-5)
        {
          Yaw.write(map(-45,-90,90,0,180));
        }
        //servomap = map(YawAngle, -90, 90, 0, 180);
        //Yaw.write(servomap);
        break;
      case 2:
        Roll1.write(90+HorizAngle);
        Roll2.write(90-HorizAngle);
        break;
    }
  }
}

void grabAngles()
{
    mpu6050.update();
  HorizAngle = mpu6050.getAngleY();
  PitchAngle = mpu6050.getAngleX();
  YawAngle = mpu6050.getAngleZ();
}
void printAngles()
{
  Serial.print("HorizAngle : ");
  Serial.println(HorizAngle);
  Serial.print("PitchAngle : ");
  Serial.println(PitchAngle);
  Serial.print("YawAngle : ");
  Serial.println(YawAngle);
}

void encodePosition()
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
