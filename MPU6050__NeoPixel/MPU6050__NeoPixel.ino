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

void setup() {
  // Start serial communication
  Serial.begin(9600);

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
    for (int i = 0; i < NUM_LEDS/2; i++) {
      strip.setPixelColor(i, 255, 0, 0); // Red
    }
    for (int i = NUM_LEDS/2; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, 0, 0, 0); // Turn off the left blinker LEDs
    }
    strip.show();
  }
  // Check if the bicycle rider is turning left
  else if (accel.acceleration.x < -0.5) {
    // Turn on the left blinker LEDs
    for (int i = 0; i < NUM_LEDS/2; i++) {
      strip.setPixelColor(i, 0, 0, 0); // Turn off the right blinker LEDs
    }
    for (int i = NUM_LEDS/2; i < NUM_LEDS; i++) {
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
