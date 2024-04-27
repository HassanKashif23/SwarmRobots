#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6SoOyGBMs"
#define BLYNK_TEMPLATE_NAME "Master"
#define BLYNK_AUTH_TOKEN "HkzG0EByQ20ro277edZq0JomEZdMhBTZ"
//#define BLYNK_TEMPLATE_ID "TMPL6GK3Edd3Y"
//#define BLYNK_TEMPLATE_NAME "ESPNOW"
//#define BLYNK_AUTH_TOKEN "nzcxYQC54Yfeaz8KUdV1uoaLN037rF0J"
#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000.h"
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4
#define IN1 0
#define IN2 16
#define IN3 21
#define IN4 22
#define D1 12
#define D2 25
#define D3 26
#define D4 32
bool forward = 0;
bool backward = 0;
bool left = 0;
 bool right = 0;
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "ssidname";
//char pass[] = "password";


float distance;
// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4;   // spi select pin
// TAG antenna delay defaults to 16384
// leftmost two bytes below will become the "short address"
char tag_addr[] = "7D:00:22:EA:82:60:3B:9C";

BlynkTimer timer;
// This function is called every time the Virtual Pin 0 state changes

void myTimerEvent()
{
  distance = DW1000Ranging.getDistantDevice()->getRange();
  Blynk.virtualWrite(V4, distance);
}

BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  forward = param.asInt();
  digitalWrite(D1,forward);
  // Update state
  //Blynk.virtualWrite(V1, value);
}
BLYNK_WRITE(V1)
{
  // Set incoming value from pin V1 to a variable
  backward = param.asInt();
  digitalWrite(D2,backward);
  // Update state
  //Blynk.virtualWrite(V1, value);
}
BLYNK_WRITE(V2)
{
  // Set incoming value from pin V2 to a variable
  left = param.asInt();
  digitalWrite(D3,left);
  // Update state
  //Blynk.virtualWrite(V1, value);
}
BLYNK_WRITE(V3)
{
  // Set incoming value from pin V3 to a variable
  right = param.asInt();
  digitalWrite(D4,right);
  // Update state
  //Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
/*void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}
*/

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

 SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
// start as tag, do not assign random short address
  DW1000Ranging.startAsTag(tag_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}


void carforward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carbackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carturnleft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carturnright() {  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop()
{
  Blynk.run();
   timer.run();
   DW1000Ranging.loop();
  if (forward == 1) {
    carforward();
  } else if (backward == 1) {
    carbackward();
  } else if (left == 1) {
    carturnleft();
  } else if (right == 1) {
    carturnright();
  } else if (forward == 0 && backward == 0 && left == 0 && right == 0) {
    carStop();
  }

  //master();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

void newRange()
{
  Serial.print("from: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print(",");
  Serial.print("\t Range: ");
  Serial.println(DW1000Ranging.getDistantDevice()->getRange());
  distance = DW1000Ranging.getDistantDevice()->getRange();

}

void newDevice(DW1000Device *device)
{
  Serial.print("Device added: ");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device)
{
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}
