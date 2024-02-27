#include "led.h"

void Led::init() {
    led.begin();
    led.setBrightness(Brightness);
    led.fill(Blank);

    led.show();
}

bool Led::setBLE_Status(bool status) {
    if (status) {
        led.setPixelColor(0, Cyan);
    } else {
        led.setPixelColor(0, Red);
    }

    return status;
}

void Led::show(void) {
    led.show();
}

void Led::setParkingStatus(int status[4]) {
    for (int id = 0; id < 4; id++) {
        for (int i = 1; i <= 2; i++) {
            switch (status[id]) {
                case 0:
                    led.setPixelColor(id * 2 + i, Cyan);
                    break;

                case 1:
                    led.setPixelColor(id * 2 + i, Red);
                    break;

                case 2:
                    bool blink = (millis() / 500) % 2;
                    led.setPixelColor(id * 2 + i, blink ? Yellow : Blank);
                    break;
            }
        }
    }
}
