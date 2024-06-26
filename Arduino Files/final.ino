#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define LED_PIN 13 // using GPIO 13 on the Pico

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 8, LED_PIN, // two 32 by 8 LED panels connected
  NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);
//arrangment of how the LEDs move from the next column or row - can take some trial and error to figure out 

// Colors array for text
const uint16_t colors[] = {
  matrix.Color(255, 20, 147),  // Pink
  matrix.Color(255, 0, 0),     // Red
  matrix.Color(255, 121, 0),   // Orange
  matrix.Color(255, 255, 0),   // Yellow
  matrix.Color(0, 255, 0),     // Green
  matrix.Color(0, 255, 255),   // Cyan
  matrix.Color(0, 0, 255),     // Blue
  matrix.Color(128, 0, 128)    // Purple
};

RTC_DS3231 rtc;

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* host = ""; // change this to your host site

void setup() {
  Wire.setSDA(0); // SDA to GPIO 0
  Wire.setSCL(1); // SCL to GPIO 1
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);

  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

String fetchMessage() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://" + String(host) + "/message.txt";
    http.begin(url);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("Received message: " + payload);
      return payload;
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  return "";
}

void loop() {
  DateTime now = rtc.now();

  int currentHour = now.hour();
  int currentMinute = now.minute();

  // check if the current time is between 7:00 AM to 5:00 PM
  bool ledOn = (currentHour >= 7 && currentHour < 17);

  if (ledOn) {
    static int pass = 0; 
    static int x = matrix.width(); 

    String message = fetchMessage();
    message.trim(); 

    int textLength = message.length();
    int textWidth = textLength * 6; // each character is 6 pixels wide
    int y = 0; // one row from the top

    // continuously scroll
    while (true) {
      matrix.fillScreen(0); // clear screen

      matrix.setTextColor(colors[pass]);
      matrix.setCursor(x, y);
      matrix.print(message);

      matrix.show(); // display on LED panel
      delay(100);    // adjust scrolling speed
      
      if (--x < -textWidth) {
        x = matrix.width(); 
        if (++pass >= sizeof(colors) / sizeof(colors[0])) pass = 0; // cycle through colors
      }

      // check time again
      DateTime now = rtc.now();
      currentHour = now.hour();
      currentMinute = now.minute();
      ledOn = (currentHour >= 7 && currentHour < 17);
      if (!ledOn) {
        matrix.fillScreen(0); // clear screen if time is out of range
        matrix.show();
        break;
      }
    }
  } else {
    // turn off the LED matrix 
    matrix.fillScreen(0);
    matrix.show();
  }
}
