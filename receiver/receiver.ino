#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>

#define enA A3
#define enB A2

Servo ESCA;
Servo ESCB;

#define lazor1 A1
#define lazor2 A0

struct Data_Package {
  byte j1PotX;
  byte j1PotY;
//  byte j1Button;
  byte j2PotX;
  byte j2PotY;
//  byte j2Button;
  byte button4;
  byte toggle1;
  byte toggle2;
  byte pot1;
  byte pot2;
};

Data_Package data;

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  
  radio.begin();
  radio.setAutoAck(false);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  ESCA.attach(enA, 1000, 2000);
  ESCB.attach(enB, 1000, 2000);

  pinMode(lazor1, OUTPUT);
  pinMode(lazor2, OUTPUT);

  data.j1PotX = 127;
  data.j1PotY = 127;
  data.toggle1 = false;
}

void loop() {
//  Serial.println("Receiving");
  if (radio.available()) {

    radio.read(&data, sizeof(Data_Package));
  }

  ESCA.write(map(data.j1PotX, 0, 255, 0, 180));
  ESCB.write(map(data.j2PotX, 0, 255, 180, 0));

  if (data.toggle1) {
    digitalWrite(lazor1, HIGH);
    digitalWrite(lazor2, HIGH);
  } else {
    digitalWrite(lazor1, LOW);
    digitalWrite(lazor2, LOW);
  }
}


