#ifndef _LED_H_
#define _LED_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Led {
   private:
    // CONFIG
    const int Brightness = 100;

    const int LedPin = 9;
    const int LedQuantity = 1;

    Adafruit_NeoPixel led =
        Adafruit_NeoPixel(LedQuantity, LedPin, NEO_GRB + NEO_KHZ800);

   public:
    void init();
    bool setBLE_Status(bool status);

    void show(void);

    const uint32_t Cyan = led.Color(0, 150, 255);
    const uint32_t Red = led.Color(255, 0, 0);
    const uint32_t Yellow = led.Color(255, 150, 0);
    const uint32_t Blank = led.Color(0, 0, 0);
};

#endif  // _LED_H_