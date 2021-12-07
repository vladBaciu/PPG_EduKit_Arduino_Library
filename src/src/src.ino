#include "PPG_EduKit.h"

typedef enum
{
    AMP = 0,
    HPF = 1,
    LPF = 2,
} channels_t;
/* @brief Class object - handler */
PPG_EduKit PPG_Shield;
/* @brief OLED custom text */
char oledText[][20] = {"AMP", "HPF",  "LPF"};
/* @brief Used for switch debouncing */
uint32_t prevSwitch[3] = {0xFFFFUL};
uint32_t curSwitch[3] = {0xFFFFUL};
/* @brief Keep track of X, Y cursor position */
volatile uint8_t displayX = 0U;
volatile uint8_t displayY = 0U;
/* @brief Keep track of active channel that is plotted */
volatile channels_t activeChannel = AMP;

static void displayFilterText(uint8_t textIndex)
{
    Adafruit_SH110X &display_reference = PPG_Shield.getHandler_OLED();
    
    display_reference.setTextSize(3);
    display_reference.setTextColor(SH110X_WHITE);
    display_reference.setCursor(40, 20);

    for(uint8_t i = 0 ; i < sizeof(oledText[textIndex % 3]) ; i++)
    {
       display_reference.print(oledText[textIndex % 3][i]);  
    }

    display_reference.display(); 
    delay(800);
    display_reference.clearDisplay();
}

static void readSwitches(void)
{
    prevSwitch[AMP] = curSwitch[AMP];
    curSwitch[AMP] = digitalRead(OLED_BUTTON_A);
    if(curSwitch[AMP] != prevSwitch[AMP])
    {
       activeChannel = AMP;
       displayFilterText(AMP);
       displayX = 0;
    } 
    prevSwitch[HPF] = curSwitch[HPF];
    curSwitch[HPF] = digitalRead(OLED_BUTTON_B);
    if(curSwitch[HPF] != prevSwitch[HPF])
    {
       activeChannel = HPF;
       displayFilterText(HPF);
       displayX = 0;
    }
    prevSwitch[LPF] = curSwitch[LPF];
    curSwitch[LPF] = digitalRead(OLED_BUTTON_C);
    if(curSwitch[LPF] != prevSwitch[LPF])
    {
       activeChannel = LPF;
       displayFilterText(LPF);
       displayX = 0;
    }
}


static uint8_t map_to_display(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {

  PPG_EK_Peripherals periphList = {
    .oledDisplay = ENABLE_PERIPHERAL,
    .neoPixel = DISABLE_PERIPHERAL,
    .tempSensor = DISABLE_PERIPHERAL,
    .ppgSensor = DISABLE_PERIPHERAL,
    .read_TIA = DISABLE_PERIPHERAL,
    .read_HPF = ENABLE_PERIPHERAL,
    .read_LPF = ENABLE_PERIPHERAL,
    .read_AMP = ENABLE_PERIPHERAL
  };

  delay(1000);
  PPG_Shield.begin(&periphList, 70);
  delay(1000);

  Adafruit_SH110X &display_reference = PPG_Shield.getHandler_OLED();
  display_reference.clearDisplay();


  PPG_Shield.enableLed(IR_CHANNEL, 5, true);

  curSwitch[AMP] = digitalRead(OLED_BUTTON_A);
  curSwitch[HPF] = digitalRead(OLED_BUTTON_B);
  curSwitch[LPF] = digitalRead(OLED_BUTTON_C);

  activeChannel = AMP;
  displayFilterText(AMP);
}

void loop() {
  Adafruit_SH110X &display_reference = PPG_Shield.getHandler_OLED();
  uint32_t bufflength = 0;
  uint16_t* cBuf = NULL;
  
  switch(activeChannel)
  {
    case AMP:
      cBuf = PPG_Shield.readChannel(ADC_AMP, &bufflength);
      break;
   case HPF:
      cBuf = PPG_Shield.readChannel(ADC_HPF, &bufflength);
      break;
   case LPF:
      cBuf = PPG_Shield.readChannel(ADC_LPF, &bufflength);
      break;  
    
  }
  
  if(bufflength != 0)
  {
    for(uint16_t i = 0; i <= bufflength; i++)
    {
      readSwitches();
      displayY = map_to_display((uint8_t)cBuf[i], 0, 200, 64, 0);
      if(displayX == 129)
      {
        displayX = 0;
        display_reference.clearDisplay();
      }
      display_reference.drawPixel(displayX, displayY, SH110X_WHITE);
      display_reference.display();
      displayX++;
      delay(10);
    }
  }
  
}
