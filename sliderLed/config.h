/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "hoda54"
#define IO_KEY       "aio_PAgq33Nlv46xZQI1xmFJJISFFKx8"
//Wi-Fi Details
#define WIFI_SSID       "Hackerman"
#define WIFI_PASS       "dmnq5761"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);