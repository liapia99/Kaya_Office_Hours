#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 13

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 8, LED_PIN,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

// Color cycle
const uint16_t colors[] = {
  matrix.Color(255, 20, 147),
  matrix.Color(255, 0, 0),
  matrix.Color(255, 121, 0),
  matrix.Color(255, 255, 0),
  matrix.Color(0, 255, 0),
  matrix.Color(0, 255, 255),
  matrix.Color(0, 0, 255),
  matrix.Color(128, 0, 128)
};

RTC_DS3231 rtc;

const char* ssid = "wifi";
const char* password = "wifi_password";

WebServer server(80);

String message = "Welcome to the IDEA Lab!";
int scrollSpeed = 100;
int brightness = 40;

// ---------------- WEB UI 

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body {
        background: black;
        color: #ff1a1a;
        font-family: Arial;
        text-align: center;
        margin-top: 40px;
      }
      input, button {
        margin: 10px;
        padding: 12px;
        font-size: 16px;
        border-radius: 8px;
        border: none;
      }
      input {
        width: 80%;
        background: #111;
        color: white;
        border: 2px solid red;
      }
      button {
        background: red;
        color: white;
        cursor: pointer;
      }
      button:hover {
        background: darkred;
      }
    </style>
  </head>
  <body>
    <h2>LED Control Panel</h2>

    <form action="/set">
      <input type="text" name="msg" placeholder="Enter message">
      <br>
      <button type="submit">Send</button>
    </form>

    <form action="/speed">
      <input type="number" name="val" placeholder="Scroll Speed (ms)">
      <button type="submit">Set Speed</button>
    </form>

    <form action="/brightness">
      <input type="number" name="val" placeholder="Brightness (0-100)">
      <button type="submit">Set Brightness</button>
    </form>

    <form action="/clear">
      <button type="submit">Clear Message</button>
    </form>

  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// --- handlers 

void handleSet() {
  if (server.hasArg("msg")) {
    message = server.arg("msg");
  }
  server.send(200, "text/html", "Message Updated<br><a href='/'>Back</a>");
}

void handleSpeed() {
  if (server.hasArg("val")) {
    scrollSpeed = server.arg("val").toInt();
  }
  server.send(200, "text/html", "Speed Updated<br><a href='/'>Back</a>");
}

void handleBrightness() {
  if (server.hasArg("val")) {
    brightness = server.arg("val").toInt();
    matrix.setBrightness(brightness);
  }
  server.send(200, "text/html", "Brightness Updated<br><a href='/'>Back</a>");
}

void handleClear() {
  message = "";
  server.send(200, "text/html", "Cleared<br><a href='/'>Back</a>");
}

// ---------------- SETUP 

void setup() {
  Serial.begin(9600);

  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();

  if (!rtc.begin()) {
    while (1);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/speed", handleSpeed);
  server.on("/brightness", handleBrightness);
  server.on("/clear", handleClear);

  server.begin();
}

// ---------------- LOOP 

void loop() {
  server.handleClient();

  DateTime now = rtc.now();
  int hour = now.hour();

  bool ledOn = (hour >= 7 && hour < 17);

  if (ledOn && message.length() > 0) {

    static int pass = 0;
    static int x = matrix.width();

    int textWidth = message.length() * 6;

    matrix.fillScreen(0);
    matrix.setTextColor(colors[pass]);
    matrix.setCursor(x, 0);
    matrix.print(message);
    matrix.show();

    delay(scrollSpeed);

    if (--x < -textWidth) {
      x = matrix.width();
      pass = (pass + 1) % (sizeof(colors) / sizeof(colors[0]));
    }

  } else {
    matrix.fillScreen(0);
    matrix.show();
  }
}
