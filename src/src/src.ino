#include "PPG_EduKit.h"

PPG_EduKit PPG_Shield;
Adafruit_SH110X *oledHandler;
frameParams_t frameParam;
unsigned int samplingRate = 200;
uint8_t *gpFrame = NULL;

void setup() {
  pinMode(24,OUTPUT);
  PPG_EK_Peripherals periphList = {
    .oledDisplay = ENABLE_PERIPHERAL,
    .neoPixel = ENABLE_PERIPHERAL,
    .tempSensor = DISABLE_PERIPHERAL,
    .ppgSensor = ENABLE_PERIPHERAL,
    .read_TIA = DISABLE_PERIPHERAL,
    .read_HPF = DISABLE_PERIPHERAL,
    .read_LPF = ENABLE_PERIPHERAL,
    .read_AMP = ENABLE_PERIPHERAL
  };
  PPG_Shield.begin(&periphList, samplingRate);

  PPG_Shield.enableLed(IR_CHANNEL, 5, true);
}

void loop() {
    Adafruit_SH110X &display_test = PPG_Shield.getHandler_OLED();
    Adafruit_NeoPixel &neoPixel = PPG_Shield.getHandler_NeoPixel();
                         neoPixel.clear();

  uint32_t bufflength = 0;
  uint16_t* cBuf = PPG_Shield.readChannel(ADC_AMP, &bufflength);
  if(bufflength != 0)
  {
  memset(&frameParam, 0x00, sizeof(frameParam));
  frameParam.frameType = CHANNEL_DATA;
  frameParam.params.wavelength = IR_CHANNEL;
  frameParam.tissueDetected = true;
  gpFrame = PPG_Shield.createSerialFrame((uint16_t *) &cBuf[0], (bufflength) * 2, &frameParam);
        
        /* Sens serial frame */
  PPG_Shield.sendFrame(gpFrame);
  }
  
}


