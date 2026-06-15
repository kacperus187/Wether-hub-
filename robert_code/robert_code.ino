#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor Configuration
#define DHTPIN 2       // AM2302 Data pin connected to D2
#define DHTTYPE DHT22  // AM2302 is a DHT22 variety
#define MQ_PIN A0      // MQ sensor Analog pin connected to A0

DHT dht(DHTPIN, DHTTYPE);
int timeGone = 0;

void setup() { 
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Halt if allocation fails
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(3);
  display.print(F("FCK NZS"));
  display.setCursor(0, 40);
  display.print(F("FCK AFD"));
  display.display();
  delay(390);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("von hanni und nanni  produziert "));
  display.display();
  
  delay(1000);
 
  dht.begin();
  display.clearDisplay();
}

void loop() {
  // Read Sensors
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasValue = analogRead(MQ_PIN);

  // Check if readings failed
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from AM2302 sensor!"));
    return;
  }

  // Display on OLED
  display.clearDisplay();

  // Header
  display.setTextSize(1);
  display.setCursor(0, 7);
  display.println(F("       FAT MAS"));
  display.drawFastHLine(0, 17, 128, WHITE);

  // Temperature & Humidity
  display.setCursor(0, 20);
  display.print(F("Temp: "));
  display.setCursor(45, 20);
  display.print(t, 1);
  display.print(F("  C"));
  display.drawFastHLine(0, 30, 128, WHITE); // Fixed typo here (changed comma to semicolon)

  display.setCursor(0, 35);
  display.print(F("feucht:  "));
  display.setCursor(45, 35);
  display.print(h, 1);
  display.print(F(" %"));
  display.drawFastHLine(0, 45, 128, WHITE);

  // MQ Sensor Value
  display.setCursor(0, 50);
  display.print(F("CO2:   "));
  display.setCursor(45, 50);
  display.print(gasValue);
  display.drawFastHLine(0, 60, 128, WHITE);
  display.setCursor(70, 50);

  // Using F() macro inside if/else to keep RAM completely clear
  if (gasValue > 2000)      { display.print(F("Rettet euch")); }
  else if (gasValue > 1500) { display.print(F(":|")); }
  else if (gasValue > 1000) { display.print(F(":(")); }
  else if (gasValue > 900)  { display.print(F(":)")); }
  else if (gasValue > 500)  { display.print(F(":D")); }
  else                      { display.print(F("T_T")); }
  
  display.display();
  Serial.print(String(timeGone) + "__" + String(t) + "__" + h + "__" + gasValue);
  Serial.println();
  delay(1000);
}
