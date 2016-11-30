#include <ESP8266WiFi.h>
ADC_MODE(ADC_VCC); //vcc read-mode

#define PRESSURE_SENSOR 1

#include <SHT21.h>
#ifdef PRESSURE_SENSOR
#include <Sodaq_BMP085.h>
#endif

#define VCC_ADJ 1.096

#define SERIAL_DEBUG 1

const char* ssid = "";
const char* password = "";

const int sleepTimeS = 15 * 60;

const char* host = "things.ubidots.com";
const char* token = "";
const char* device = "";

SHT21 sht;
#ifdef PRESSURE_SENSOR
Sodaq_BMP085 bmp;
#endif

void setup() 
{
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  Serial.println("Wake up...");
#endif
  
  sht.begin();
#ifdef PRESSURE_SENSOR
  bmp.begin(BMP085_STANDARD);
#endif

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef SERIAL_DEBUG
    Serial.print(".");
#endif
  }

#ifdef SERIAL_DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to ");
  Serial.println(host);
#endif
  
  WiFiClient client;
  int retries = 5;
  const int httpPort = 80;
  while (!client.connect(host, httpPort) && (retries-- > 0)) {
#ifdef SERIAL_DEBUG
    Serial.print(".");
#endif
  }

  float temperature = sht.getTemperature();
  float humidity = sht.getHumidity();
#ifdef PRESSURE_SENSOR
  float pressure = (float) bmp.readPressure() / 100.0;
#endif
  float voltage = ESP.getVcc() * VCC_ADJ / 1000.0;

#ifdef SERIAL_DEBUG
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

#ifdef PRESSURE_SENSOR
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
#endif

  Serial.print("Battery: ");
  Serial.print(voltage);
  Serial.println(" V");
#endif

  String url = "/api/v1.6/devices/";

  String values = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + 
#ifdef PRESSURE_SENSOR
                  ", \"pressure\":" + String(pressure) + 
#endif
                  ", \"battery\":" + String(voltage) + "}";

  String urlLoad = String("POST ") + url + device +
                   "/ HTTP/1.1\r\n" +
                   "X-Auth-Token: " + token + "\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: "  + values.length() + "\r\n\r\n" +
                   values + "\r\n\r\n";

#ifdef SERIAL_DEBUG
  Serial.print(urlLoad);
#endif

  client.print(urlLoad);

  int timeout = 5 * 10;
  while (!client.available() && (timeout-- > 0)) {
    delay(100);
  }

  if (!client.available()) {
#ifdef SERIAL_DEBUG
    Serial.println("No response from server...");
#endif
  }

#ifdef SERIAL_DEBUG
  Serial.println("Sleep...");
#endif
  ESP.deepSleep(sleepTimeS * 1000000);  
}

void loop() 
{
}

