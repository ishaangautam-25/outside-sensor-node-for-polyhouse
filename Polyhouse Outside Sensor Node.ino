#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <DFRobot_SHT20.h>  // Include DFRobot SHT20 library
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>  // Include SoftwareSerial library for MHZ19C
#include <ErriezMHZ19B.h>    // Include MHZ19B library (adapted for MH-Z19C)
#include <ESP8266WiFi.h>     // Include Wi-Fi library
#include <ThingSpeak.h>      // Include ThingSpeak library

// TSL2591 object
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

// SHT20 object
DFRobot_SHT20 sht20;

// DS18B20 temperature sensors
#define ONE_WIRE_BUS_1 D5  // DS18B20 sensor 1 on D5
#define ONE_WIRE_BUS_2 D6  // DS18B20 sensor 2 on D6
OneWire oneWire1(ONE_WIRE_BUS_1);
OneWire oneWire2(ONE_WIRE_BUS_2);
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);

// PCA9548A I2C MUX address
#define MUX_ADDRESS 0x70

// Select the MUX channel, 0-7
void selectMuxChannel(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void configureSensor() {
  tsl.setGain(TSL2591_GAIN_MED);  // Medium gain
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // 100ms integration time
}

// Pin definitions for MH-Z19C
#define MHZ19C_TX_PIN D3
#define MHZ19C_RX_PIN D4

// Create SoftwareSerial object for MH-Z19C
SoftwareSerial mhzSerial(MHZ19C_RX_PIN, MHZ19C_TX_PIN);

// Create MHZ19B object (adapted for MH-Z19C)
ErriezMHZ19B mhz19c(&mhzSerial);

// Pin definitions for YFS201A water flow sensors
#define FLOW_SENSOR_1_PIN D7
#define FLOW_SENSOR_2_PIN D8

// Variables for water flow sensors
volatile uint32_t pulseCount1 = 0;
volatile uint32_t pulseCount2 = 0;
unsigned long lastFlowCalculation1 = 0;
unsigned long lastFlowCalculation2 = 0;
float flowRate1 = 0.0;
float flowRate2 = 0.0;

// Wi-Fi credentials
const char* ssid = "IIT-Mandi-Wifi MIC";
const char* password = "wifi@iitMIC";

// ThingSpeak settings
unsigned long myChannelNumber = 2597577;  // Replace with your channel number
const char* myWriteAPIKey = "9XYOI0XHDJ67JI0R";  // Replace with your Write API Key

// LED pin
#define LED_PIN D0

void ICACHE_RAM_ATTR pulseCounter1() {
  pulseCount1++;
}

void ICACHE_RAM_ATTR pulseCounter2() {
  pulseCount2++;
}

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);  // SDA, SCL

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Select MUX channel 0 for TSL2591
  selectMuxChannel(0);
  
  if (!tsl.begin()) {
    Serial.println(F("No TSL2591 detected ... Check your wiring or I2C ADDR!"));
    while (1);
  }
  
  Serial.println(F("Found TSL2591 sensor"));
  
  // Configure the TSL2591 sensor
  configureSensor();
  
  // Select MUX channel 1 for SHT20
  selectMuxChannel(1);
  
  // Initialize the SHT20 sensor
  sht20.initSHT20();
  
  Serial.println(F("Found SHT20 sensor"));
  
  // Initialize DS18B20 sensors
  sensors1.begin();
  sensors2.begin();
  
  Serial.println(F("Found DS18B20 sensors"));

  // Initialize MH-Z19C CO2 sensor
  mhzSerial.begin(9600);  // Start serial communication for MH-Z19C
  while (!mhz19c.detect()) {
    Serial.println(F("Detecting MH-Z19C sensor..."));
    delay(2000);
  }

  // Sensor warming up
  while (mhz19c.isWarmingUp()) {
    Serial.println(F("Warming up..."));
    delay(2000);
  }

  Serial.print(F("MH-Z19C CO2 sensor ready"));

  // Initialize flow sensors
  pinMode(FLOW_SENSOR_1_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_1_PIN), pulseCounter1, FALLING);

  pinMode(FLOW_SENSOR_2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_2_PIN), pulseCounter2, FALLING);

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // Select MUX channel 0 for TSL2591
  selectMuxChannel(0);
  
  // Get TSL2591 sensor event
  sensors_event_t event;
  tsl.getEvent(&event);
  
  // Print TSL2591 sensor values
  float light = 0.0;
  if (event.light) {
    light = event.light;
    Serial.print(F("Lux: "));
    Serial.println(event.light);
  } else {
    Serial.println(F("Sensor overload"));
  }
  
  // Select MUX channel 1 for SHT20
  selectMuxChannel(1);
  
  // Read SHT20 sensor data
  float temperature = sht20.readTemperature();
  float humidity = sht20.readHumidity();
  
  // Print SHT20 sensor values
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println(" %");
  
  // Read DS18B20 sensor 1
  sensors1.requestTemperatures();
  float temp1 = sensors1.getTempCByIndex(0);
  Serial.print(F("DS18B20 Sensor 1 Temperature: "));
  Serial.print(temp1);
  Serial.println(" °C");
  
  // Read DS18B20 sensor 2
  sensors2.requestTemperatures();
  float temp2 = sensors2.getTempCByIndex(0);
  Serial.print(F("DS18B20 Sensor 2 Temperature: "));
  Serial.print(temp2);
  Serial.println(" °C");

  // Read MH-Z19C CO2 sensor data
  int CO2_ppm = mhz19c.readCO2();
  Serial.print(F("MH-Z19C CO2: "));
  Serial.print(CO2_ppm);
  Serial.println(" ppm");

  // Calculate and print flow rates
  if (millis() - lastFlowCalculation1 > 1000) {  // Update every second
    flowRate1 = (pulseCount1 / 7.5);  // Calculation for flow rate in L/min (depends on sensor specs)
    pulseCount1 = 0;
    lastFlowCalculation1 = millis();
    Serial.print(F("Flow rate 1: "));
    Serial.print(flowRate1);
    Serial.println(F(" L/min"));
  }

  if (millis() - lastFlowCalculation2 > 1000) {  // Update every second
    flowRate2 = (pulseCount2 / 7.5);  // Calculation for flow rate in L/min (depends on sensor specs)
    pulseCount2 = 0;
    lastFlowCalculation2 = millis();
    Serial.print(F("Flow rate 2: "));
    Serial.print(flowRate2);
    Serial.println(F(" L/min"));
  }

  // Send data to ThingSpeak
  ThingSpeak.setField(1, light);
  ThingSpeak.setField(2, flowRate1);
  ThingSpeak.setField(3, flowRate2);
  ThingSpeak.setField(4, temp1);
  ThingSpeak.setField(5, temp2);
  ThingSpeak.setField(6, temperature);
  ThingSpeak.setField(7, humidity);
  ThingSpeak.setField(8, CO2_ppm);

  // Turn on LED before sending data
  digitalWrite(LED_PIN, HIGH);

  // Write data to ThingSpeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // Turn off LED after data is sent
  digitalWrite(LED_PIN, LOW);

  delay(1000);  // Wait 10 seconds to update the channel again
}