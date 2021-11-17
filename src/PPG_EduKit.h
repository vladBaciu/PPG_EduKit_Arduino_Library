#include "Arduino.h"
#include <memory>

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include <Protocentral_MAX30205.h>
#include <MAX30105.h>


#define     ENABLE_PERIPHERAL         (0xAA)
#define     DISABLE_PERIPHERAL        (0x00)

#define ADC_SAMPLE_RATE           (128UL)
#define I2C_SPEED_STANDARD        100000
#define I2C_SPEED_FAST            400000

#define OLED_I2C_ADDRESS    0x3CU
#define OLED_BUTTON_A       9U
#define OLED_BUTTON_B       8U
#define OLED_BUTTON_C       7U

#define TLC5925_LE          2U
#define TLC5925_OE          5U
#define TLC5925_CLK         3U
#define TLC5925_SDI         4U

#define AD5273_I2C_ADDRESS  0x2CU
#define AD5273_R_W_VALUE    60U
#define AD5273_R_AB_VALUE   10000UL

#define MAX30105_LED_BRIGHTNESS      60U
#define MAX30105_SAMPLE_AVERAGE      4U
#define MAX30105_LED_MODE            2U
#define MAX30105_SAMPLE_RATE         100UL
#define MAX30105_PUSLE_WIDTH         411UL
#define MAX30105_ADC_RANGE           16384UL

#define NEOPIXEL_PIN                 0U
#define SWITCH_BUTTON                1U

#define MAX_BUFFER_SIZE              (200UL)

typedef enum{
    RED_LED,
    GREEN_LED,
    IR_LED
} PPG_EK_Led;

typedef struct{

    uint8_t oledDisplay;
    uint8_t neoPixel;
    uint8_t tempSensor;
    uint8_t ppgSensor;
    uint8_t read_TIA;
    uint8_t read_HPF;
    uint8_t read_LPF;
    uint8_t read_AMP;

} PPG_EK_Peripherals;


class PPG_EduKit {

    public:
        PPG_EduKit(void) : display(64, 128, &Wire1), pixels(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800) 
        {
            _display = (std::unique_ptr<Adafruit_SH110X>) &display;
            _pixels = (std::unique_ptr<Adafruit_NeoPixel>) &pixels;
            _tempSensor = (std::unique_ptr<MAX30205>) &tempSensor;
            _ppgSensor = (std::unique_ptr<MAX30105>) &ppgSensor;
        }

        void begin(PPG_EK_Peripherals *peripheralsList);
        void enableLed(PPG_EK_Led ledType, uint16_t ledCurrent, boolean setCurrent);

        static void ADC_HandlerISR();

        Adafruit_SH110X& getHandler_OLED() { return *(this->_display); }
        Adafruit_NeoPixel& getHandler_NeoPixel() { return *(this->_pixels); }
#ifndef __SAM3X8E__
        MAX30205& getHandler_TempSensor() { return *(this->_tempSensor); }
#endif
        MAX30105& getHandler_PpgSensor() { return *(this->_ppgSensor); }
        

        volatile static  uint16_t   PPG_EduKit_TIA_Buffer[MAX_BUFFER_SIZE];
        volatile static  uint16_t   PPG_EduKit_HPF_Buffer[MAX_BUFFER_SIZE];
        volatile static  uint16_t   PPG_EduKit_LPF_Buffer[MAX_BUFFER_SIZE];
        volatile static  uint16_t   PPG_EduKit_AMP_Buffer[MAX_BUFFER_SIZE];
        volatile static  uint16_t   PPG_EduKIT_BufferHead;

        static  uint8_t numberOfActiveChannels;
        static  uint8_t adcChannels[4];
        
    private:
        TwoWire *_i2cPort;
        Adafruit_SH110X display;
        Adafruit_NeoPixel pixels;        
        MAX30205 tempSensor;
        MAX30105 ppgSensor;
        
        std::unique_ptr<Adafruit_SH110X> _display;
        std::unique_ptr<Adafruit_NeoPixel> _pixels;
        std::unique_ptr<MAX30205> _tempSensor;
        std::unique_ptr<MAX30105> _ppgSensor;

        void OLED_displaySetup(void);
        void TLC5925_enableRed(void);
        void TLC5925_enableGreen(void);
        void TLC5925_enableIR(void);
        void AD5273_setLedCurrent(uint16_t val);
        void ADC_Init(uint8_t channels);


};
