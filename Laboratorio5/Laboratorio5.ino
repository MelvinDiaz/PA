#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define IO_USERNAME  "hoda54"
#define IO_KEY       "aio_PAgq33Nlv46xZQI1xmFJJISFFKx8"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);

Adafruit_MQTT_Publish adafruitInfrared = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/infrarrojo", MQTT_QOS_1);

#define WLAN_SSID       "Hackerman"
#define WLAN_PASS       "dmnq5761"

#define sensorInfrarrojo 4

void MQTT_connect();
void connectToWifi();

void setup() {
  Serial.begin(9600);
  connectToWifi();

}

void loop() {
  MQTT_connect();
  float infrared = digitalRead(sensorInfrarrojo);

  if(infrared == 0)
      adafruitInfrared.publish("Objeto detectado");
  else
      adafruitInfrared.publish("Objeto no detectado");

  delay(2000);
}


void connectToWifi(){
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void readSensorInfo(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    adafruitTemperature.publish(event.temperature);
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    adafruitHumidity.publish(event.relative_humidity);
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}


void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}