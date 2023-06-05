#include <Arduino.h>
#include <Adafruit_TCS34725.h>
#include <Orange_inferencing.h>


static bool debug_nn = false;
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
float orange, pear;
void loop() {
  ei_printf("\Starting in 2 seconds...\n");
  delay(2000);
  ei_printf("Sampling...\n");
  float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
  for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
    float r, g, b;
    uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);
    tcs.getRGB(&r, &g, &b);
    buffer[ix] = r;
    buffer[ix + 1] = g;
    buffer[ix + 2] = b;
    delayMicroseconds(next_tick - micros());
  }
  signal_t signal;
  int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
  if (err != 0) {
    ei_printf("Failed to create signal from buffer (%d)\n", err);
    return;
  }
  ei_impulse_result_t result = { 0 };

  err = run_classifier(&signal, &result, debug_nn);
  if (err != EI_IMPULSE_OK) {
    ei_printf("ERR: Failed to run classifier (%d)\n", err);
    return;
  }
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    //ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
    if (!strcmp(result.classification[ix].label, "Orange")) {
      if (result.classification[ix].value >= 0.5) {
        ei_printf("Orange\n");
      } else {
        ei_printf("Pear\n");
      }
    }
    if (!strcmp(result.classification[ix].label, "Pear")) {
      if (result.classification[ix].value >= 0.5) {
        ei_printf("Pear\n");
      } else {
        ei_printf("Orange\n");
      }
    }
  }
}