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

#define MAX_ERROR_RETRY_COUNT 3
unsigned int errorCounter = MAX_ERROR_RETRY_COUNT;
const int errorSleepTimeS = 1 * 60;
const int sleepTimeS = 15 * 60;

const char* server = "api.thingspeak.com";
String writeAPIKey = "";

SHT21 sht;
#ifdef PRESSURE_SENSOR
Sodaq_BMP085 bmp;
#endif

void error(void)
{
  errorCounter--;

  if (errorCounter > 0) {
    ESP.rtcUserMemoryWrite(0, (unsigned int*) &errorCounter, sizeof(unsigned int));

#ifdef SERIAL_DEBUG
    Serial.println("Sleeping for 1 minute...");
#endif
    ESP.deepSleep(errorSleepTimeS * 1000000);
  } else {
    errorCounter = MAX_ERROR_RETRY_COUNT;
    ESP.rtcUserMemoryWrite(0, (unsigned int*) &errorCounter, sizeof(unsigned int));

#ifdef SERIAL_DEBUG
    Serial.println("Sleeping for 15 minutes...");
#endif
    ESP.deepSleep(sleepTimeS * 1000000);
  }
}

void setup()
{
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  Serial.println("Waking up...");
#endif

  ESP.rtcUserMemoryRead(0, (unsigned int*) &errorCounter, sizeof(unsigned int));
  if (errorCounter > MAX_ERROR_RETRY_COUNT) {
    errorCounter = MAX_ERROR_RETRY_COUNT;
  }

#ifdef SERIAL_DEBUG
    Serial.print("Error counter: ");
    Serial.println(errorCounter);
#endif

  sht.begin();
#ifdef PRESSURE_SENSOR
  bmp.begin(BMP085_STANDARD);
#endif

  WiFi.setOutputPower(0);
  WiFi.begin(ssid, password);

#ifdef SERIAL_DEBUG
  Serial.print("Connecting to ");
  Serial.print(ssid);
#endif
  int timeout = 30; // 15 seconds wifi connect timeout
  while ((WiFi.status() != WL_CONNECTED) && (timeout-- > 0)) {
    delay(500);
#ifdef SERIAL_DEBUG
    Serial.print(".");
#endif
  }
#ifdef SERIAL_DEBUG
  Serial.println("");
#endif

  if (timeout < 0) {
#ifdef SERIAL_DEBUG
    Serial.println("ERROR: Failed to connect!");
#endif
    error();
  }

#ifdef SERIAL_DEBUG
  Serial.print("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to ");
  Serial.print(server);
#endif

  WiFiClient client;

  int retries = 5;
  while (!client.connect(server, 80) && (retries-- > 0)) {
#ifdef SERIAL_DEBUG
    Serial.print(".");
#endif
  }
#ifdef SERIAL_DEBUG
  Serial.println("");
#endif

  if (retries < 0) {
#ifdef SERIAL_DEBUG
    Serial.println("ERROR: Failed to connect!");
#endif
    error();
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

#ifdef SERIAL_DEBUG
  Serial.print("Waiting for reply");
#endif
  timeout = 5 * 10; // 5 seconds timeout
  while (!client.available() && (timeout-- > 0)) {
    delay(100);
#ifdef SERIAL_DEBUG
    Serial.print(".");
#endif
  }
#ifdef SERIAL_DEBUG
  Serial.println("");
#endif

  if (timeout < 0) {
#ifdef SERIAL_DEBUG
    Serial.println("ERROR: No response from server!");
#endif
    error();
  }

  if (!client.available()) {
#ifdef SERIAL_DEBUG
    Serial.println("No response from server...");
#endif
  }

  errorCounter = MAX_ERROR_RETRY_COUNT;
  ESP.rtcUserMemoryWrite(0, (unsigned int*) &errorCounter, sizeof(unsigned int));

#ifdef SERIAL_DEBUG
  Serial.println("Sleeping for 15 minutes...");
#endif
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop()
{
}
