#include <ESP8266WiFi.h>

extern "C" {
#include <espnow.h>
}

typedef struct {
  int buttonState;
} Message;

Message data;

// Callback when data is received
void receiveData(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&data, incomingData, sizeof(data));

  Serial.print("Received: ");
  Serial.println(data.buttonState);

  // Built-in LED is ACTIVE LOW
  if (data.buttonState == 1)
  {
    digitalWrite(LED_BUILTIN, LOW);   // LED ON
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);  // LED OFF
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0)
  {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

  esp_now_register_recv_cb(receiveData);

  Serial.println("Receiver Ready");
}

void loop()
{
}
