#include "PPG_EduKit.h"

PPG_EduKit PPG_Shield;


unsigned int samplingRate = 125;

frameParams_t frameParam;
uint8_t *gpFrame = NULL;

void setup() {
 
  PPG_EK_Peripherals periphList = {
    .oledDisplay = DISABLE_PERIPHERAL,
    .neoPixel = DISABLE_PERIPHERAL,
    .tempSensor = DISABLE_PERIPHERAL,
    .ppgSensor = DISABLE_PERIPHERAL,
    .read_TIA = DISABLE_PERIPHERAL,
    .read_HPF = DISABLE_PERIPHERAL,
    .read_LPF = DISABLE_PERIPHERAL,
    .read_AMP = ENABLE_PERIPHERAL
  };
  
  PPG_Shield.begin(&periphList, samplingRate);

  PPG_Shield.enableLed(IR_CHANNEL, 5, true);
}

void loop() {

  uint32_t bufflength = 0;
  uint16_t* cBuf = PPG_Shield.readChannel(ADC_AMP, &bufflength);
  
  if(bufflength != 0)
  {
    memset(&frameParam, 0x00, sizeof(frameParam));
    frameParam.frameType = CHANNEL_DATA;
    frameParam.params.wavelength = IR_CHANNEL;
    frameParam.tissueDetected = true;
    gpFrame = PPG_Shield.createSerialFrame((uint16_t *) &cBuf[0], (bufflength) * 2, &frameParam);
             
    PPG_Shield.sendFrame(gpFrame);
  }
  
}
