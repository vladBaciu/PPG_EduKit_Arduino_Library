#include "PPG_EduKit.h"

PPG_EduKit PPG_Shield;

char oledText[][30] = {"LAB1: I2C interfacing", "Temperature: "};

float temperature;
float lastTemperature;
char ascii_temp[3];




static void displayStrings(void)
{
    Adafruit_SH110X &display_reference = PPG_Shield.getHandler_OLED();
    
    display_reference.setTextSize(1);
    display_reference.setTextColor(SH110X_WHITE);
    display_reference.setCursor(0, 10);
    /* Display "LAB1: I2C interfacing" */
    for(uint8_t i = 0 ; i < sizeof(oledText[0]) ; i++){
            display_reference.print(oledText[0][i]);  
      }
    display_reference.setCursor(0, 30);
    /* Display "Temperature: " */
    for(uint8_t i = 0 ; i < sizeof(oledText[1]) ; i++){
            display_reference.print(oledText[1][i]);  
    }
    display_reference.display(); 
}

static void refreshTempValue(void)
{
    int whole = temperature;
    int remainder  = (temperature - whole) * 100;
    
    Adafruit_SH110X &display_reference = PPG_Shield.getHandler_OLED();
    /* Clear old value */
    for(uint8_t i = 70; i < 120; i++)
    {
        for(uint8_t j = 30; j < 50 ; j++)
            display_reference.drawPixel(i, j, SH110X_BLACK);
    }
    display_reference.display();
    /* Prepare to display new value */
    display_reference.setCursor(80, 30);
    /* Convert dec number to ASCII value */
    PPG_Shield.int2acii(temperature, ascii_temp);
    /* Draw the ASCII value and store it in _displaybuf */
    for(uint8_t i = 0 ; i < sizeof(ascii_temp) ; i++){
        display_reference.print(ascii_temp[i]);  
    }
    display_reference.setCursor(90, 30);
    display_reference.print(".");  
     /* Prepare to display new value */
    display_reference.setCursor(97, 30);
    /* Convert dec number to ASCII value */
    PPG_Shield.int2acii(remainder, ascii_temp);
    /* Draw the ASCII value and store it in _displaybuf */
    for(uint8_t i = 0 ; i < sizeof(ascii_temp) ; i++){
        display_reference.print(ascii_temp[i]);  
    }
    display_reference.display();
}

void setup() {
  
  PPG_EK_Peripherals periphList = {
    .oledDisplay = ENABLE_PERIPHERAL,
    .neoPixel = DISABLE_PERIPHERAL,
    .tempSensor = ENABLE_PERIPHERAL,
    .ppgSensor = DISABLE_PERIPHERAL,
    .read_TIA = DISABLE_PERIPHERAL,
    .read_HPF = DISABLE_PERIPHERAL,
    .read_LPF = DISABLE_PERIPHERAL,
    .read_AMP = DISABLE_PERIPHERAL
  };

  delay(1000);
  PPG_Shield.begin(&periphList, 0);
  delay(1000);

  Adafruit_SH110X &display_reference = PPG_Shield.getHandler_OLED();
  display_reference.clearDisplay();

  displayStrings();
}

void loop() {

  lastTemperature = temperature;
  temperature = PPG_Shield.MAX30205_GetTemperature();
  
  if(temperature != lastTemperature)
  {
      refreshTempValue();
      Serial.println(temperature);
      delay(500);
  }
}


