#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

  // Buttons
  pinMode(3, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  radio.begin();
  radio.setAutoAck(false);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

  drawText("Starting...");

//  radio.printDetails();
}

void loop() {
//  Serial.print("Is fail: ");
//  Serial.println(radio.failureDetected);
  data.j1PotX = map(analogRead(A6), 0, 1023, 100, 150);
  data.j1PotY = map(analogRead(A7), 0, 1023, 100, 150);
  data.j2PotX = map(analogRead(A3), 0, 1023, 100, 150);
  data.j2PotY = map(analogRead(A2), 0, 1023, 100, 150);
  data.pot1 = map(analogRead(A1), 0, 1023, 0, 255);
  data.pot2 = map(analogRead(A0), 0, 1023, 0, 255);
  data.button4 = digitalRead(3);
  data.toggle1 = digitalRead(7);
  data.toggle2 = digitalRead(2);

//Serial.print(data.j1PotX);
//Serial.print(", ");
//Serial.print(data.j1PotY);
//Serial.print(", ");
//Serial.print(data.j2PotX);
//Serial.print(", ");
//Serial.print(data.j2PotY);
//Serial.print(", ");
//Serial.print(data.pot1);
//Serial.print(", ");
//Serial.print(data.pot2);
//Serial.print(", ");
//Serial.print(data.button4 == HIGH ? 100 : 0);
//Serial.print(", ");
//Serial.print(data.toggle1 == HIGH ? 100 : 0);
//Serial.print(", ");
//Serial.print(data.toggle2 == HIGH ? 100 : 0);
//Serial.println("");

//  unsigned long current_time = micros();
//  String timeString = "Time :";
//  timeString += current_time;
//  char text[32];
//  timeString.toCharArray(text, 32);
////  char text[] = "Time " + String(current_time, DEC);
//  radio.write(&text, sizeof(text));
//  radio.write(&current_time, sizeof(unsigned long));

  radio.write(&data, sizeof(Data_Package));

  drawText("X: "+ String(data.j1PotX) +" Y: "+ String(data.j1PotY));

  delay(100);

////  Serial.print("X-axis: ");
//  Serial.print(analogRead(X_pin));
//  Serial.print(",");
////  Serial.print("\n");
////  Serial.print("Y-axis: ");
//  Serial.println(analogRead(Y_pin));
////  Serial.print("\n\n");
//  delay(500);
}

void drawText(String text) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(text);
  display.display();
  delay(100);
}

