#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 13 // Using GPIO 13 on the Pico

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 8, LED_PIN, // 2 32 by 8 LED Panels 
  NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

// Colors array for text
const uint16_t colors[] = {
  matrix.Color(255, 20, 147),  // Pink
  matrix.Color(255, 0, 0),     // Red
  matrix.Color(255, 165, 0),   // Orange
  matrix.Color(255, 255, 0),   // Yellow
  matrix.Color(0, 255, 0),     // Green
  matrix.Color(0, 255, 255),   // Cyan
  matrix.Color(0, 0, 255),     // Blue
  matrix.Color(128, 0, 128)    // Purple
};

RTC_DS3231 rtc;

void setup() {
  // Initialize I2C0 communication on GPIO 0 (SDA) and GPIO 1 (SCL)
  Wire.setSDA(0); // Set SDA to GPIO 0
  Wire.setSCL(1); // Set SCL to GPIO 1
  Wire.begin();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialize LED matrix
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  DateTime now = rtc.now();

  int currentHour = now.hour();
  int currentMinute = now.minute();

  // Check if the current time is within the specified range (7:50 AM to 5:00 PM)
  bool ledOn = (currentHour > 7 && currentHour < 17) || (currentHour == 7 && currentMinute >= 50);

  if (ledOn) {
    static int pass = 0; 
    static int x = matrix.width(); // Initial x position off-screen

    if (Serial.available()) {
      String inputString = Serial.readStringUntil('\n'); 
      inputString.trim(); 

      // Calculate text length and width based on user input
      int textLength = inputString.length();
      int textWidth = textLength * 6; // Each character is 6 pixels wide
      int y = 0; // Second row from the top

      // Continuously scroll the text
      while (true) {
        matrix.fillScreen(0); // Clear screen for new frame

        // Print text at current position
        matrix.setTextColor(colors[pass]);
        matrix.setCursor(x, y);
        matrix.print(inputString);

        matrix.show(); // Display on LED panel
        delay(100);    // Adjust scrolling speed

        // Scroll text leftward
        if (--x < -textWidth) {
          x = matrix.width(); 
          if (++pass >= sizeof(colors) / sizeof(colors[0])) pass = 0; // Cycle through colors
        }

        if (Serial.available()) {
          break; 
        }

        // Check time again within the loop
        DateTime now = rtc.now();
        currentHour = now.hour();
        currentMinute = now.minute();
        ledOn = (currentHour > 7 && currentHour < 17) || (currentHour == 7 && currentMinute >= 50);
        if (!ledOn) {
          matrix.fillScreen(0); // Clear screen if time is out of range
          matrix.show();
          break;
        }
      }
    }
  } else {
    // Turn off the LED matrix if out of specified time range
    matrix.fillScreen(0);
    matrix.show();
  }
}
