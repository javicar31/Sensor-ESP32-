//My head hurts but i learnt a lot with this project. 
//Code By: Javier Carnicer
//Created on: May 4 2024

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "";
const char* password = "";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // I2C address for the SSD1306 display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4     // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11   // Type of the DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// Offset correction for humidity (its around 10 so i put 10 .-. need to test more)
float humidityOffset = 10.0; // Add a 10% offset to the humidity readings

// Offset correction for temperature
float temperatureOffset = -6.0; // Subtract 6 degrees Fahrenheit from the temperature readings

WebServer server(80);

void setup() {
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);  // Delay for stabilizing display
  display.clearDisplay();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    display.println("Connecting to WiFi...");
  }
  display.clearDisplay();
  Serial.println("Connected to WiFi");
  display.println("Connected to WiFi");
  dht.begin();

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  float temperatureC, temperatureF, humidity;
  
  temperatureC = dht.readTemperature(); // Read temperature value in Celsius
  temperatureF = (temperatureC * 9.0 / 5.0) + 32.0; // Convert Celsius to Fahrenheit
  humidity = dht.readHumidity(); // Read humidity value
  
  if (isnan(temperatureC) || isnan(temperatureF) || isnan(humidity)) {
    Serial.println("You Suck at wiring dude!");
    return;
  }
  
  // Apply offset correction to humidity
  humidity += humidityOffset;
  // Apply offset correction to temperature
  temperatureF += temperatureOffset;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.print(temperatureF);
  display.println(" F");

  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  // Display IP address
  display.println();
  display.print("IP: ");
  display.println(WiFi.localIP());
  
  display.display();
  delay(2000); // Delay before next reading
}

void handleRoot() {
  float temperatureC, temperatureF, humidity;
  
  temperatureC = dht.readTemperature(); // Read temperature value in Celsius
  temperatureF = (temperatureC * 9.0 / 5.0) + 32.0; // Convert Celsius to Fahrenheit
  humidity = dht.readHumidity(); // Read humidity value
  
  if (isnan(temperatureC) || isnan(temperatureF) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Apply offset correction to humidity
  humidity += humidityOffset;
  // Apply offset correction to temperature
  temperatureF += temperatureOffset;
  
  String webpage = "<html><head><meta http-equiv='refresh' content='5'></head><body>";
  webpage += "<h1> This is a test website made to display temperature and humidity from an esp32 using a DH11 sensor<h1>";
  webpage += "<h1>Temperature: " + String(temperatureF) + " F</h1>";
  webpage += "<h1>Humidity: " + String(humidity) + " %</h1>";
  webpage += "<h1>By: JC </h1>";// to add IP + WiFi.localIP().toString() + 
  webpage += "</body></html>";
  
  server.send(200, "text/html", webpage);
}
