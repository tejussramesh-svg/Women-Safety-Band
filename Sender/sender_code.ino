#include <ESP8266WiFi.h>

extern "C" {
#include <espnow.h>
}

typedef struct {
  int buttonState;
} Message;

Message data;

// Receiver MAC Address
uint8_t receiverMAC[] = {0xC8, 0x2B, 0x96, 0x22, 0x89, 0x22};

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Send Status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void setup()
{
  Serial.begin(115200);

  pinMode(D2, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0)
  {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_register_send_cb(OnDataSent);

  esp_now_add_peer(receiverMAC,
                   ESP_NOW_ROLE_SLAVE,
                   1,
                   NULL,
                   0);
}

void loop()
{
  data.buttonState = !digitalRead(D2);

  esp_now_send(receiverMAC,
               (uint8_t *)&data,
               sizeof(data));

  Serial.print("Sent: ");
  Serial.println(data.buttonState);

  delay(500);
}
