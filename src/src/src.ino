#include "PPG_EduKit.h"

PPG_EduKit PPG_Shield;
Adafruit_SH110X *oledHandler;

void setup() {
  
  PPG_EK_Peripherals periphList = {
    .oledDisplay = ENABLE_PERIPHERAL,
    .neoPixel = ENABLE_PERIPHERAL,
    .tempSensor = DISABLE_PERIPHERAL,
    .ppgSensor = ENABLE_PERIPHERAL,
    .read_TIA = DISABLE_PERIPHERAL,
    .read_HPF = DISABLE_PERIPHERAL,
    .read_LPF = DISABLE_PERIPHERAL,
    .read_AMP = DISABLE_PERIPHERAL
  };
  PPG_Shield.begin(&periphList);

  PPG_Shield.enableLed(RED_LED, 5, true);

}

void loop() {
    Adafruit_SH110X &display_test = PPG_Shield.getHandler_OLED();
    Adafruit_NeoPixel &neoPixel = PPG_Shield.getHandler_NeoPixel();
                         neoPixel.clear();

                     neoPixel.setPixelColor(0, neoPixel.Color(150, 0, 0));

                     neoPixel.show();

                     delay(4000);

                        neoPixel.clear();

                     neoPixel.setPixelColor(0, neoPixel.Color(150, 150, 0));

                     neoPixel.show();

                     delay(4000);
                    PPG_Shield.PPG_EduKIT_BufferHead++;

}
