#include <ESP8266WiFi.h>
ADC_MODE(ADC_VCC); //vcc read-mode

// #define PRESSURE_SENSOR 1

#include <SHT21.h>
#ifdef PRESSURE_SENSOR
#include <Sodaq_BMP085.h>
#endif

#define VCC_ADJ 1.035

// #define SERIAL_DEBUG 1

const char* ssid = "";
const char* password = "";

const int sleepTimeS = 15 * 60;

const char* server = "api.thingspeak.com";
String writeAPIKey = "";

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

  WiFi.setOutputPower(0);
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
  Serial.println(server);
#endif

  WiFiClient client;
  int retries = 5;

  while (!client.connect(server, 80) && (retries-- > 0)) {
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

  String body = "field1=" + String(temperature) + ", &field2=" + String(humidity) +
                "&field3=" + String(voltage);
#ifdef PRESSURE_SENSOR
  body += "&field4=" + String(pressure);
#endif

  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(body.length());
  client.print("\n\n");
  client.print(body);
  client.print("\n\n");

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
