
#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t Slave1Address[] = {0xD4,0xD4,0xDA,0x46,0x72,0x50};
uint8_t Slave2Address[] = {0xD4,0xD4,0xDA,0x44,0x1F,0xF0};
int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;
typedef struct struct_message {
  bool a;
  bool b;
  bool c;
  bool d;
} struct_message;
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 500;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(D1,INPUT);
  pinMode(D2,INPUT);
  pinMode(D3,INPUT);
  pinMode(D4,INPUT);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(Slave1Address, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(Slave2Address, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
   state1 = digitalRead(D1);
   state2 = digitalRead(D2);
   state3 = digitalRead(D3);
   state4 = digitalRead(D4);
   if(state1 == HIGH)
    {
      myData.a = true;
    }
    else
    {
      myData.a = false;
    }
    if(state2 == HIGH)
    {
      myData.b = true;
    }
    else
    {
      myData.b = false;
    }
    if(state3 == HIGH)
    {
      myData.c = true;
    }
    else
    {
      myData.c = false;
    }
    if(state4 == HIGH)
    {
      myData.d = true;
    }
    else
    {
      myData.d = false;
    }
    esp_now_send(Slave1Address, (uint8_t *) &myData, sizeof(myData));
    esp_now_send(Slave2Address, (uint8_t *) &myData, sizeof(myData));
    lastTime = millis();
  }
}
