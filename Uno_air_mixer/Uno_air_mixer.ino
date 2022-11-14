/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <icp101xx.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor is an ICP101xx object
ICP101xx sensor;

void setup() {
  sensor.begin();
  Serial.begin(9600);

  sensor.measureStart(sensor.VERY_ACCURATE);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();
  delay(100);

}

void loop() {
  //Average pressure & temperature readings over 10 values
  float pressureSum = 0;
  float pressure = 0;
  
  float temperatureSum = 0;
  float temperature = 0;
  
  for (int i=0; i<10; i++) {
      // When the measurement is complete, dataReady() will return true.
      if (sensor.dataReady()) {
        // Read and output measured temperature in Celcius and pressure in Pascal.
        Serial.print(sensor.getTemperatureC());
        Serial.print(",");
        Serial.println(sensor.getPressurePa());
        temperature = sensor.getTemperatureC();
        pressure = sensor.getPressurePa();

        // Start the next measurement cycle.
        sensor.measureStart(sensor.VERY_ACCURATE);
      }
      pressureSum = pressureSum + pressure;
      temperatureSum = temperatureSum + temperature;
      delay(100);
  }

  float pressureAvg = pressureSum/10;
  float temperatureAvg = temperatureSum/10;

  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Pressure:"));
  display.println(pressureAvg);
  display.setTextSize(1);
  display.println();
  display.setTextSize(2);
  display.print(temperatureAvg);
  display.println(F("'C"));
  display.display();

  delay(100);
  
}

