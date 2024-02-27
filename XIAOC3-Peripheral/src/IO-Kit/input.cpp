#include "Arduino.h"
#include "IO-Kit.h"

Input::Input(int pin) {
    this->_pinNumber = pin;
    pinMode(this->_pinNumber, INPUT);
}

Input::Input(int pin, bool pullup) {
    this->_pinNumber = pin;
    if (pullup) {
        pinMode(this->_pinNumber, INPUT_PULLUP);
    } else {
        pinMode(this->_pinNumber, INPUT);
    }
}

bool Input::digital(void) {
    return digitalRead(this->_pinNumber);
}

double Input::analog(void) {
    return double(analogRead(this->_pinNumber)) / 4096.0;
}

int Input::raw(void) {
    return analogRead(this->_pinNumber);
}

Input::operator bool() {
    return digital();
}