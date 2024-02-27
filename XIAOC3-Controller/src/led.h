#ifndef _LED_H_
#define _LED_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Led {
   private:
    // CONFIG
    const int Brightness = 100;

    const int ledPin = 20;
    const int ledQuantity = 8;

    Adafruit_NeoPixel led =
        Adafruit_NeoPixel(1 + ledQuantity, ledPin, NEO_GRB + NEO_KHZ800);

   public:
    void init();

    // status | 0: available, 1: occupied , 2: reserved
    void setParkingStatus(int status[4]);
    bool setBLE_Status(bool status);

    void show(void);

    const uint32_t Cyan = led.Color(0, 150, 255);
    const uint32_t Red = led.Color(255, 0, 0);
    const uint32_t Yellow = led.Color(255, 150, 0);
    const uint32_t Blank = led.Color(0, 0, 0);
};

#endif  // _LED_H_