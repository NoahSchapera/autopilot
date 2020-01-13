#include <MPU6050_tockn.h>


#include <Servo.h>
#include <Wire.h>

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

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  
  ServoSetup();
  MPUSetup();
  onlineBlink();
}


void loop() 
{
  grabAngles();
  printAngles();
  
  for(int c = 0; c < 3; c++)
  {
    switch(c)
    {
      case 0:
        servomap = map(PitchAngle, -90, 90, 0, 180);
        Pitch.write(servomap);
        break;
      case 1:
        servomap = map(YawAngle, -90, 90, 0, 180);
        Yaw.write(servomap);
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
