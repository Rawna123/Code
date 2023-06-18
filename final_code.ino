#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define NUM_LEDS 61
#define LED_BRIGHTNESS 100

// MPU6050 object
Adafruit_MPU6050 mpu;

// Neopixel object
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

//gsm
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;
char MESSAGE[300];
char lat[12];
char lon[12];
char wspeed[12];
char phone[16];
char datetime[24];
#define PIN_TX    10
#define PIN_RX    9
SoftwareSerial mySerial(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&mySerial);
char msg;
void setup() {
  // Start serial communication
  Serial.begin(9600);
  mySerial.begin(9600);
  while (!sim808.init())
  {
    Serial.print("Sim808 init error\r\n");
    delay(1000);
  }
  delay(3000);

  if ( sim808.attachGPS())
    Serial.println("Open the GPS power success");
  else
    Serial.println("Open the GPS power failure");

  Serial.println("Init Success, please send SMS message to me!");
  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050!");
    while (1);
  }

  // Initialize Neopixels
  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  strip.show();

}

void loop() {
  // Get the acceleration data from MPU6050
  SendgpsMessage();
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp); // Provide all 3 arguments

  // Print out the acceleration data
  Serial.print("Acceleration (m/s^2): X=");
  Serial.print(accel.acceleration.x);
  Serial.print(" Y=");
  Serial.print(accel.acceleration.y);
  Serial.print(" Z=");
  Serial.println(accel.acceleration.z);

  // Check if the bicycle rider is turning right
  if (accel.acceleration.x > 0.5) {
    // Turn on the right blinker LEDs
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      strip.setPixelColor(i, 255, 0, 0); // Red
    }
    for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, 0, 0, 0); // Turn off the left blinker LEDs
    }
    strip.show();
  }
  // Check if the bicycle rider is turning left
  else if (accel.acceleration.x < -0.5) {
    // Turn on the left blinker LEDs
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      strip.setPixelColor(i, 0, 0, 0); // Turn off the right blinker LEDs
    }
    for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, 255, 0, 0); // Red
    }
    strip.show();
  }
  // Turn off all the LEDs if no direction
  else {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, 0, 0, 0); // Turn off all the LEDs
    }
    strip.show();
  }
}
void SendgpsMessage()
{
  messageIndex = sim808.isSMSunread();
  if (messageIndex > 0)
  {
    sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
    sim808.deleteSMS(messageIndex);
    while (!sim808.getGPS())
    {
    }
    float la = sim808.GPSdata.lat;
    float lo = sim808.GPSdata.lon;
    float ws = sim808.GPSdata.speed_kph;
    dtostrf(la, 6, 2, lat);
    dtostrf(lo, 6, 2, lon);
    dtostrf(ws, 6, 2, wspeed);
    sprintf(MESSAGE, "Latitude : %s\nLongitude : %s\nTry With This Link.\nhttp://www.latlong.net/Show-Latitude-Longitude.html", lat, lon);
    sim808.sendSMS(phone, MESSAGE);
    //sim808.detachGPS();
  }
}
