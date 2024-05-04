#include <DHT.h>

#define DHTPIN 4     // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11   // Type of the DHT sensor.

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);  // Delay to stabilize sensor

  float humidity = dht.readHumidity(); // Read humidity value
  float temperature = dht.readTemperature(); // Read temperature value in Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
}
