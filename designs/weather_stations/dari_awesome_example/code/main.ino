// Settings file
// #include "settings.h"

// Libs
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <ArduinoJson.h>
#include <SPI.h>

#include <HTTPClient.h>
#include <WiFi.h>

// Pin mappings

#define TFT_CS 18
#define TFT_RST 5
#define TFT_DC 17

// If you want to redefine spi pins
#define TFT_MOSI 16
#define TFT_SCLK 4

#define SSID "The Hotel"
#define WIFI_PSK "bettertobeapiratethanjointhenavy"

#define CELSIUS_URL                                                                                                    \
    "https://api.open-meteo.com/v1/"                                                                                   \
    "forecast?latitude=43.7001&longitude=-79.4163&current=temperature_2m,relative_humidity_2m,apparent_temperature,"   \
    "is_day,precipitation,rain,showers,snowfall,weather_code,cloud_cover,pressure_msl,surface_pressure,wind_speed_"    \
    "10m,wind_direction_10m,wind_gusts_10m"
#define FAHRENHEIT_URL                                                                                                 \
    "https://api.open-meteo.com/v1/"                                                                                   \
    "forecast?latitude=43.7001&longitude=-79.4163&current=temperature_2m,relative_humidity_2m,apparent_temperature,"   \
    "is_day,precipitation,rain,showers,snowfall,weather_code,cloud_cover,pressure_msl,surface_pressure,wind_speed_"    \
    "10m,wind_direction_10m,wind_gusts_10m&temperature_unit=fahrenheit&wind_speed_unit=mph&precipitation_unit=inch"


/* redefine pins here later for wemos c3 mini
#define MOSI
#define MISO
#define SCK
*/

// change between F and C
int unitSwitch = 25;

#ifdef TFT_MOSI
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
#endif

void setup()
{
    Serial.begin(115200);

    pinMode(unitSwitch, INPUT);

    // tft.initR(INITR_BLACKTAB);
    Serial.println(F("Initialized"));
    WiFi.begin(SSID, WIFI_PSK);
    Serial.println(F("\nConnecting"));

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(F("."));
        delay(100);
    }

    Serial.println(F("\nConnected to the WiFi network"));
    drawWeatherData();
}

void loop()
{

}

void drawWeatherData()
{

    JsonDocument doc;

    String input = getWeatherData();

    Serial.println(input);

    deserializeJson(doc, input);

    JsonObject current = doc["current"];

    float temperature = current["temperature_2m"];
    Serial.print("temp: ");
    Serial.println(temperature);
}

String getWeatherData()
{
    HTTPClient http;

    if (unitSwitch)
    {
        http.begin(CELSIUS_URL);
    }
    else
    {
        http.begin(FAHRENHEIT_URL);
    }

    int httpCode = http.GET();

    if (httpCode > 0)
    {
        String payload = http.getString();
        Serial.println(payload);
        return payload;
    }
    else
    {
        Serial.println(F("Error on HTTP request"));
    }

    http.end();
}
