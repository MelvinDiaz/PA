//Includes for ESP8266, DHT AND MQTT FOR ADAFRUIT IO
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//Info for wifi and account of adafruit
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define IO_USERNAME  "hoda54"
#define IO_KEY       "aio_PAgq33Nlv46xZQI1xmFJJISFFKx8"
//Important to adafruit io
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);

Adafruit_MQTT_Publish adafruitHumidity = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/Humedad", MQTT_QOS_1);
Adafruit_MQTT_Publish adafruitTemperature = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/Temperature", MQTT_QOS_1);
Adafruit_MQTT_Publish adafruitLuminosity = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/lecturaLuminosidad", MQTT_QOS_1);
Adafruit_MQTT_Publish adafruitCondition = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/condicionInvernadero", MQTT_QOS_1);



void MQTT_connect();
void connectToWifi();
void readSensorInfo();


//ssid and password
#define WLAN_SSID       "Hackerman"
#define WLAN_PASS       "dmnq5761"
//DHT information
#define DHTPIN 2     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 22 (AM2302)
#define led 13

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  Serial.begin(9600);
  // Initialize device.
  pinMode(led, OUTPUT);
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  //DHT code
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  // Set delay between sensor readings
  //delayMS = sensor.min_delay / 1000;
  delayMS = 7000;

//Connecting to wifi
  connectToWifi();

}

void loop() {
  // Connect to mqtt
  MQTT_connect();
  delay(delayMS);


  // Delay between measurements.
  
  int valorLeido = analogRead(A0);
  float voltaje = (float)valorLeido/1023*3.3;

  adafruitLuminosity.publish(voltaje);

  // Get temperature event and print its value.
  readSensorInfo();
}



//Auxiliary functions

void readSensorInfo(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    adafruitTemperature.publish(event.temperature);
    if (event.temperature > 25.0) {
      adafruitCondition.publish("El invernadero esta en peligro");
      digitalWrite(led, HIGH);
    }
    else {
    adafruitCondition.publish("SIN PELIGRO");
      digitalWrite(led, LOW);
    }
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
    if (event.relative_humidity > 80.0) {
      adafruitCondition.publish("El invernadero esta en peligro");
      digitalWrite(led, HIGH);
    }
     else {
    adafruitCondition.publish("SIN PELIGRO");
      digitalWrite(led, LOW);
    }
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }}


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
