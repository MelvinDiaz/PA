#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define IO_USERNAME  "hoda54"
#define IO_KEY       "aio_PAgq33Nlv46xZQI1xmFJJISFFKx8"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);

Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/onoff", MQTT_QOS_1);
Adafruit_MQTT_Publish adafruitTemperature = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/humedad", MQTT_QOS_1);

#define WLAN_SSID       "Hackerman"
#define WLAN_PASS       "dmnq5761"

#define DHTPIN 2     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

void MQTT_connect();
void connectToWifi();
bool flag = false;

void onoffcallback(char *data, uint16_t len) {
  


  Serial.println(data);
  String message = String(data);
  message.trim();

  if(message == "ON"){
    flag = true;
    
    Serial.println("ON");
  }
  else{
    flag = false;
    
    Serial.println("OFF");
  }
    
}


void setup() {
  Serial.begin(9600);
  dht.begin();
  connectToWifi();
  onoffbutton.setCallback(onoffcallback);
  mqtt.subscribe(&onoffbutton);
}

void loop() {
  MQTT_connect();

  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  Serial.print("TEMPERATURA ");
  Serial.println(t);
  Serial.print("TEMPERATURA EN FAREN");
  Serial.println(f);
  

  if(flag)
    adafruitTemperature.publish(f);
  else
    adafruitTemperature.publish(t);

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