#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

RF24 transmit (7,8);                    //create RF24 object called transmit

byte address [6] = "00001";             //set address to 00001

void setup() {
  Serial.begin(9600);
  transmit.begin();
  transmit.openWritingPipe(address);    //open writing pipe to address 00001
  transmit.setPALevel(RF24_PA_MIN);     //set RF power output to minimum
  transmit.setDataRate(RF24_250KBPS);   //set datarate to 250kbps
  transmit.setChannel(100);             //set frequency to channel 100
  transmit.stopListening();
   }

void loop() {
 Serial.println("Sending Data");
  char data[32] = "It Works !";         //send character string upto 32 bytes long
  transmit.write(&data,sizeof(data));   //transmit the data
  delay(1000);      
}
