#include <Arduino.h>
#include <Adafruit_TCS34725.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

void setup() {
  Serial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Founded sensor!");
  } else {
    Serial.println("ERROR");
    while (1)
      ;
  }
}
uint16_t red, green, blue;
void loop() {

  float red, green, blue;
  tcs.getRGB(&red, &green, &blue);
  Serial.print(int(red));
  Serial.print(",");
  Serial.print(int(green));
  Serial.print(",");
  Serial.print(int(blue));
  Serial.print("\n");
  delay(100);
}