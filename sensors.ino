#include <SPI.h>
#include <WiFiNINA.h>

#include <DHT.h>
#include <DHT_U.h>
#include <ThingSpeak.h>
// WiFi credentials
char ssid[] = "Verizon_3V6MNV-IoT";
char pass[] = "ewe6-elude-few";

// Constants for sensor pins
#define DHTPIN 2      // DHT11 sensor pin
#define UV_PIN A0      // UV sensor pin

// Constants for ThingSpeak
char thingSpeakApiKey[] = "KTTPUOISXY6Q7E4I";
int channelNumber = 2349977;

WiFiClient  client;


// DHT sensor
DHT dht(DHTPIN, DHT11);

// Variables to store sensor readings
float temperature, humidity, uvIndex;

void setup() {
  Serial.begin(9600);
  ThingSpeak.begin(client);

  // Connect to Wi-Fi
  connectWiFi();


  // Start DHT sensor
  dht.begin();
}

void loop() {
  // Read sensor data
  readSensors();


  // Update ThingSpeak channel
  updateThingSpeak();

  // Delay for a while before looping
  delay(6000); // Delay for 1 minute
}

void readSensors() {
  // Read temperature and humidity from DHT sensor
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(true);

  // Read UV index from sensor
  /*  uvIndex = analogRead(UV_PIN);*/
  /*  uvIndex = map(uvIndex, 0, 1023, 0, 15);  // Assuming UV sensor range is 0-15*/
  
/*  int sensorValue=analogRead(A2);
  float Vsig= sensorValue*3.3/1023;;
  float uvIndex= 307*Vsig/200;
*/
  
/*  int sensorValue = analogRead(A2);
  float voltage = sensorValue * (5.0 / 1023.0); // Convert to voltage
  // Calculate Illumination Intensity
  float illuminationIntensity = 307 * voltage;
  // Calculate UV Index
  float uvIndex = illuminationIntensity / 200;*/
  
  long  sum=0; int sensorValue;
    for(int i=0;i<1024;i++)// accumulate readings for 1024 times
    {
        sensorValue=analogRead(A2);
        sum=sensorValue+sum;
        delay(2);
    }
    long meanVal = sum/1024;  // get mean value
    uvIndex = (meanVal*1000/4.3-83)/21;// get a detailed calculating expression for UV index in schematic files.

}

void updateThingSpeak() {
  // Update ThingSpeak channel with sensor and control data
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, uvIndex);

  // Write to ThingSpeak
  int status = ThingSpeak.writeFields(channelNumber, thingSpeakApiKey);
  Serial.print("The current temperature is:");
  Serial.print(temperature);// get a detailed calculating expression for UV index in schematic files.
  Serial.print("\n");
  Serial.print("The current humidity is:");
  Serial.print(humidity);// get a detailed calculating expression for UV index in schematic files.
  Serial.print("\n");
  Serial.print("The current UV index is:");
  Serial.print(uvIndex,15);// get a detailed calculating expression for UV index in schematic files.
  Serial.print("\n");

  // Check for successful write
  if (status != 200) {
    Serial.println("Failed to update ThingSpeak!");
  }
}

void connectWiFi() {
  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
}
