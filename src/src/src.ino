#include "PPG_EduKit.h"
#include "ADCSampler.h"

PPG_EduKit PPG_Shield;
Adafruit_SH110X *oledHandler;
ADCSampler sampler;

unsigned int samplingRate = 128;

void setup() {
  pinMode(24,OUTPUT);
  PPG_EK_Peripherals periphList = {
    .oledDisplay = ENABLE_PERIPHERAL,
    .neoPixel = ENABLE_PERIPHERAL,
    .tempSensor = DISABLE_PERIPHERAL,
    .ppgSensor = ENABLE_PERIPHERAL,
    .read_TIA = DISABLE_PERIPHERAL,
    .read_HPF = DISABLE_PERIPHERAL,
    .read_LPF = DISABLE_PERIPHERAL,
    .read_AMP = ENABLE_PERIPHERAL
  };
  PPG_Shield.begin(&periphList);

  PPG_Shield.enableLed(RED_LED, 5, true);
  sampler.begin(samplingRate);
}

void loop() {
    Adafruit_SH110X &display_test = PPG_Shield.getHandler_OLED();
    Adafruit_NeoPixel &neoPixel = PPG_Shield.getHandler_NeoPixel();
                         neoPixel.clear();

  

}

void ADC_Handler() {
  digitalWrite(24, !digitalRead(24));
  sampler.handleInterrupt();
}
