#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 12

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 8, PIN, // 2 32 by 8 LED Panels 
  NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

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

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);

  Serial.begin(9600);
}

void loop() {
  static int pass = 0; 
  static int x = matrix.width(); // Initial x position off-screen

  if (Serial.available()) {
    String inputString = Serial.readStringUntil('\n'); 
    inputString.trim(); 

    // Calculate text length and width based on user input
    int textLength = inputString.length();
    int textWidth = textLength * 6; // Each character is 6 pixels wide
    int y = 1; // Second row from the top

    // Continuously scroll the text
    while (true) {
      matrix.fillScreen(0); // Clear screen for new frame

      // Print text at current position
      matrix.setTextColor(colors[pass]);
      matrix.setCursor(x, y);
      matrix.print(inputString);

      matrix.show(); //Display on LED panel
      delay(100);    // Adjust scrolling speed

      // Scroll text leftward
      if (--x < -textWidth) {
        x = matrix.width(); 
        if (++pass >= sizeof(colors) / sizeof(colors[0])) pass = 0; // Cycle through colors
      }

      
      if (Serial.available()) {
        break; 
      }
    }
  }
}
